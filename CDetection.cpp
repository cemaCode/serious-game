#include "CDetection.h"

Scalar rougeBas(126, 104, 58);
Scalar rougeHaut(240, 235, 209);

Scalar vertBas(27, 105, 111);
Scalar vertHaut(60, 204, 255);

using namespace std;
using namespace cv;

//Constructeur par defaut 
CDetection::CDetection()
{
	//Initialisation de la structure à 0
    detection_struct->X_droite = 0;
    detection_struct->Y_droite = 0;
    detection_struct->X_gauche = 0;
    detection_struct->Y_gauche = 0;
    detection_struct->contact = 0;

    capture_video = new VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1640, height=(int)1232, format=(string)NV12, framerate=(fraction)30/1 ! nvvidconv flip-method=4 ! video/x-raw, width=(int)820, height=(int)616, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", CAP_GSTREAMER);
    
    if (!capture_video->isOpened()) //On verifie que la camera est ouverte
    {
        cout << "ERREUR : IMPOSSIBLE DOUVRIR LA CAMERA !"<< endl;
        exit(-1);
    }
    else
    {
        cout << "Camera ouverte" << endl;
    }
}
//Surcharge de constructeur 
CDetection::CDetection(detect_t &P_detection_struct)
{
    detection_struct = &P_detection_struct;

    capture_video = new VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1640, height=(int)1232, format=(string)NV12, framerate=(fraction)30/1 ! nvvidconv flip-method=4 ! video/x-raw, width=(int)820, height=(int)616, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", CAP_GSTREAMER); //capture the video from web cam

    if (!capture_video->isOpened()) //On verifie que la camera est ouverte
    {
        cout << "ERREUR : IMPOSSIBLE DOUVRIR LA CAMERA !"<< endl;
        exit(-1);
    }
    else
    {
        cout << "Camera ouverte" << endl;
    }
}


// Fonction pour créer l'image de seuil pour une certaine couleur
Mat creationSeuil(Mat imgHSVOrigine, Scalar seuilMin, Scalar seuilMax)
{
    Mat imgSeuil;

    inRange(imgHSVOrigine, seuilMin, seuilMax, imgSeuil); //Creation de l'image de seuil

    //Ouverture morphologique (enlève les petits objets sur le premier plan)
    erode(imgSeuil, imgSeuil, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgSeuil, imgSeuil, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //Fermeture morphologique (enlève les petits trous sur le premier plan)
    dilate(imgSeuil, imgSeuil, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgSeuil, imgSeuil, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
   
    return imgSeuil;
}

//Fonction pour detecter le marquer à partir de l'image de seuil
void detectionCouleur(Mat imageSeuil, int *positions)
{

    //On calcule les 'moments' de l'image seuil 
    Moments oMoments = moments(imageSeuil);

   	//Variables contenant les coordonnées et l'aire du 'moment' detecté
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    int posX;
    int posY;

    // On vérifie que l'aide est suppérieure à 100 px
    	//Si non on considère qu'il s'agit du bruit 
    if (dArea > 100)
    {
        //On calcule la position du marqueur coloré
        posX = dM10 / dArea;
        posY = dM01 / dArea;
        positions[0] = posX;
        positions[1] = posY;
    }
    else // Si aucun objet n'est detecté on renvoie -1
    {
        positions[0] = -1;
        positions[1] = -1;
    }
}

//Methode permettant de lancer la vidéo et l'afficher pour le positionnement
void CDetection::afficherVideo()
{
    while (true)
    {
        Mat image;
        bool a = capture_video->read(image); //On capture l'image
        imshow("Image Originale", image); //On affiche l'image
        waitKey(1);
    }
}

//Methode pour detecter les deux couleurs prédefinies ( rouge et vert )
detect_t *CDetection::detecterCouleur()
{
    Mat imgOriginal;
    bool bSuccess = capture_video->read(imgOriginal); //On récupère une image du flux video

    if (!bSuccess) //Si la capture echoue on quitte
    {
        cout << "ERREUR : Impossible de lire la video" << endl;
        exit(-1);
    }

    Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //On convertit l'image originale du BGR au HSV

    //Detetion de la couleur rouge
    Mat imageSeuilRouge = creationSeuil(imgHSV, rougeBas, rougeHaut); //On cree l'image de seuil
    int pos_rouge[] = {0, 0}; //Liste contenant les coordonées 
    detectionCouleur(imageSeuilRouge, pos_bleu); // On obtient les coordonées 

    //Detetion de la couleur verte
    Mat imageSeuilVert = creationSeuil(imgHSV, vertBas, vertHaut);
    int pos_vert[] = {0, 0};
    detectionCouleur(imageSeuilVert, pos_vert);

    //Remplissage de la structure de detection
    detection_struct->X_droite = pos_bleu[0];
    detection_struct->Y_droite = pos_bleu[1];
    detection_struct->X_gauche = pos_vert[0];
    detection_struct->Y_gauche = pos_vert[1];
    
    return detection_struct;
}

//Destructeur
CDetection::~CDetection()
{
	//Si la capture a été lancée
    if (capture_video->isOpened())
    {
        capture_video->release(); // On ferme la camera 
    } 
}
