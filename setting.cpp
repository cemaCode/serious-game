#include "setting.h"

/***************Constructeur / Destructeur**************/
Setting::Setting(QWidget *parent) : QDialog(parent), ui(new Ui::Setting)
{
    ui->setupUi(this);
    setWindowTitle(NameSettingWindows);
}
Setting::~Setting()
{
    delete ui;
}


/*************** Lit les paramètres dans le fichier Setting.txt **************/
bool MainWindow::StartSetting()
{
    bool        resultat                = false             ;
    QString     Buffer                                      ;
    QString     BufferPourIpServeur                         ;
    QString     NomPositionnementCarte  = "CheminCarte = \"";
    QStringList ListeElements                               ;

    /******* Récupère le nom du fichier paramètre *********/
    QString CheminSetting = QCoreApplication::applicationDirPath();
    CheminSetting.append("\\Setting.txt");
    CheminSetting = cheminMachine(CheminSetting, Fichier);
    QFile fileSetting(CheminSetting);

    /*********** Ouvre le fichier paramètre ***********/
    if(fileSetting.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /**** Lit toutes les lignes du fichier ****/
        Buffer = fileSetting.readAll();

        /**** Coupe les éléments du fichier ****/
        ListeElements =  Buffer.split("\"");

        /**** Récupère les informations des différents paramètres ****/
        S_Setting.CheminCarte = ListeElements[1];
        S_Setting.LinkfileStat = ListeElements[3];
        S_Setting.LinkSaveStat = ListeElements[5];
        S_Setting.PortCarte = ListeElements[7];

        /**** Récupère l'adresse IP du serveur à partir du chemin inscrit en paramètre ****/
        ListeElements =  Buffer.split("\\");
        BufferPourIpServeur = ListeElements[2];
        BufferPourIpServeur.remove(NomPositionnementCarte,Qt::CaseInsensitive);
        S_Setting.IP_Serveur = BufferPourIpServeur;

        resultat = true;
    }
    return resultat;
}

/********************** Modifie le fichier Setting.txt par de nouveaux paramètres ***************/
void Setting::ChangeParametre(QString P_Parametre)
{
    QByteArray  Buffer[4]           ;
    QString     BufferPourIpServeur ;
    QString     NomPositionnementCarte = "CheminCarte = \"";
    QStringList ListeElements       ;

    /***** Si Un paramètre a était identifié pour être modifié *****/
    if(ParametreAChanger.compare("",Qt::CaseSensitive) != 0)
    {
        /******* Récupère le nom du fichier paramètre *********/
        QString CheminSetting = QCoreApplication::applicationDirPath();
        CheminSetting.append("\\Setting.txt");
        CheminSetting = cheminMachine(CheminSetting, Fichier);
        QFile FichierStat(CheminSetting);

        /*********** Ouvre le fichier paramètre ***********/
        if(FichierStat.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            /******** Lit toutes les lignes du fichier paramètres *********/
            Buffer[0] = FichierStat.readLine();
            Buffer[1] = FichierStat.readLine();
            Buffer[2] = FichierStat.readLine();
            Buffer[3] = FichierStat.readLine();

            /******** Si le paramètre à modier est le chemin de la carte *********/
            if(P_Parametre == "CardLink")
            {
                S_Setting->CheminCarte = ParametreAChanger;
                ParametreAChanger = cheminMachine(ParametreAChanger, Dossier);
                Buffer[0].clear();
                Sleep(10);
                Buffer[0].insert(0,"CheminCarte = \"");
                Buffer[0].insert(Buffer[0].size(),ParametreAChanger);
                Buffer[0].append("\"\n");
            }

            /******** Si le paramètre à modier est le chemin des statistiques *********/
            if(P_Parametre == "StatLink")
            {
                S_Setting->LinkfileStat = ParametreAChanger;
                ParametreAChanger = cheminMachine(ParametreAChanger, Fichier);
                Buffer[1].clear();
                Sleep(10);
                Buffer[1].insert(Buffer[1].size(),"StatLink = \"");
                Buffer[1].insert(Buffer[1].size(),ParametreAChanger);
                Buffer[1].append("\"\n");
            }

            /******** Si le paramètre à modier est le chemin de sauvegarde des statistiques *********/
            if(P_Parametre == "StatSaveLink")
            {
                S_Setting->LinkSaveStat = ParametreAChanger;
                ParametreAChanger = cheminMachine(ParametreAChanger, Dossier);
                Buffer[2].clear();
                Sleep(10);
                Buffer[2].insert(Buffer[2].size(),"StatSaveLink = \"");
                Buffer[2].insert(Buffer[2].size(),ParametreAChanger);
                Buffer[2].append("\"\n");
            }

            /******** Si le paramètre à modier est le port du serveur *********/
            if(P_Parametre == "PortCarte")
            {
                S_Setting->PortCarte = ParametreAChanger;
                Buffer[3].clear();
                Sleep(10);
                Buffer[3].insert(Buffer[3].size(),"PortCarte = \"");
                Buffer[3].insert(Buffer[3].size(),ParametreAChanger);
                Buffer[3].append("\"\n");
            }

            /******** Ecrit les nouveaux paramètres *********/
            FichierStat.resize(0);
            FichierStat.write(Buffer[0]);
            FichierStat.write(Buffer[1]);
            FichierStat.write(Buffer[2]);
            FichierStat.write(Buffer[3]);

            /**** Récupère l'adresse IP du serveur à partir du chemin inscrit en paramètre ****/
            BufferPourIpServeur = Buffer[0];
            ListeElements =  BufferPourIpServeur.split("\\");
            BufferPourIpServeur = ListeElements[2];
            BufferPourIpServeur.remove(NomPositionnementCarte,Qt::CaseInsensitive);
            S_Setting->IP_Serveur = BufferPourIpServeur;

            /**** Ferme le fichier paramètre ****/
            FichierStat.close();
        }
    }
}

/*********************** Affichage des paramètres *********************/
void Setting::AfficheParametre()
{
    QString Buffer;

    /***** Récupère et adapte le chemin de la carte puis l'affiche *****/
    Buffer = S_Setting->CheminCarte;
    Buffer = cheminMachine(Buffer, Dossier);
    this->on_CarLink_textChanged(Buffer);

    /***** Récupère et adapte le chemin du fichier statistique puis l'affiche *****/
    Buffer = S_Setting->LinkfileStat;
    Buffer.replace(S_Setting->CheminCarte,"\\",Qt::CaseInsensitive);
    this->on_StatLink_textChanged(Buffer);

    /***** Récupère et adapte le chemin de sauvegarde des statistiques puis l'affiche *****/
    Buffer = S_Setting->LinkSaveStat;
    Buffer.replace(S_Setting->CheminCarte,"\\",Qt::CaseInsensitive);
    this->on_StatSaveLink_textChanged(Buffer);

    /***** Récupère le port du serveur puis l'affiche *****/
    Buffer = S_Setting->PortCarte;
    this->on_PortCarte_textChanged(Buffer);
}


/************************************************************ SLOTS ************************************************************************/

/******** Affiche le chemin de la carte ***********/
void Setting::on_CarLink_textChanged(const QString &arg1)
{
   ui->CarLink->setText(arg1);
}

/******** Affiche le chemin du fichier statistique ***********/
void Setting::on_StatLink_textChanged(const QString &arg1)
{
    ui->StatLink->setText(arg1);
}

/******** Affiche le chemin de sauvegarde des statistiques ***********/
void Setting::on_StatSaveLink_textChanged(const QString &arg1)
{
    ui->StatSaveLink->setText(arg1);
}

/******** Affiche le port du serveur ***********/
void Setting::on_PortCarte_textChanged(const QString &arg1)
{
    ui->PortCarte->setText(arg1);
}

/******** Bouton "Modifier" du chemin de la carte ***********/
void Setting::on_ChangeCardLink_clicked()
{
    QString dossierCourant = QFileDialog::getExistingDirectory(this);
    if(!dossierCourant.isNull())
    {
        ParametreAChanger = cheminMachine(dossierCourant, Dossier);
        ChangeParametre("CardLink");
        AfficheParametre();
    }
}

/******** Bouton "Modifier" du chemin du fichier statistique ***********/
void Setting::on_ChangeStatLink_clicked()
{
    QString dossierCourant = QFileDialog::getExistingDirectory(this);
    if(!dossierCourant.isNull())
    {
        ParametreAChanger = cheminMachine(dossierCourant, Dossier);
        ChangeParametre("StatLink");
        AfficheParametre();
    }
}

/******** Bouton "Modifier" du chemin de sauvegarde des statistiques ***********/
void Setting::on_ChangeStatSaveLink_clicked()
{
    QString dossierCourant = QFileDialog::getExistingDirectory(this);
    if(!dossierCourant.isNull())
    {
        ParametreAChanger = cheminMachine(dossierCourant, Dossier);
        ChangeParametre("StatSaveLink");
        AfficheParametre();
    }
}

/******** Bouton "Modifier" du port du serveur ***********/
void Setting::on_ChangePortCard_clicked()
{
    ParametreAChanger = ui->PortCarte->text();
    ChangeParametre("PortCarte");
    AfficheParametre();
}
