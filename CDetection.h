#ifndef CDETECTION_H
#define CDETECTION_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

//Structure de detection
typedef struct detect
{
    int X_droite;
    int Y_droite;
    int X_gauche;
    int Y_gauche;
    int contact;
} detect_t;

class CDetection
{
private:
	VideoCapture* capture_video; // Handle sur la camera
    detect_t* detection_struct; // Structure de detection
public:
	CDetection();
	CDetection(detect_t &P_detection_struct);
    detect_t* detecterCouleur(); //Methode pour la detection
	void afficherVideo(); //Methode pour le positionnement 
    ~CDetection();
};

#endif //CDETECTION_H
