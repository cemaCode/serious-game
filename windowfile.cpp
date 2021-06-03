#include "windowfile.h"
#include "ui_windowfile.h"
#include "setting.h"

/********************************Constructeur***************************************/
WindowFile::WindowFile(QWidget *parent) : QDialog(parent),  ui(new Ui::WindowFile)
{
    ui->setupUi(this);

    // Affiche au lancement l'arboressence pour récupérer une musique
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    dirmodel->setRootPath(CheminComplet);
    ui->treeView->setModel(dirmodel);

    // Cache les 3 collums de bases
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    // Augmente la taille de al colum name
    ui->treeView->setColumnWidth(0,250);

}
WindowFile::~WindowFile()
{
    delete ui;
}




/********************************Click***************************************/
void WindowFile::on_ChoiceLink_textChanged(const QString &arg1)
{
    ui->ChoiceLink->setText(arg1);
}
void WindowFile::on_ChangeLink_clicked()
{
    close();
}





// A travailler!!!

    //QString dossierCourant = QFileDialog::getExistingDirectory(this);


/********************************TREE VIEW***************************************/
void WindowFile::on_treeView_doubleClicked(const QModelIndex &index)
{
    // Récupération du chemin
    CheminComplet = dirmodel->fileInfo(index).absolutePath();
    // Récupération du nom de la musique
    QString FichierTexte = dirmodel->fileName(index);
    if(FileOrDir == Fichier)
    {
        // Assemblage du chemin et du nom
        CheminComplet = CheminComplet.insert(CheminComplet.size(), "/");
        CheminComplet = CheminComplet.insert(CheminComplet.size(), FichierTexte);
    }
    on_ChoiceLink_textChanged(CheminComplet);
}
void WindowFile::on_treeView_clicked(const QModelIndex &index)
{
    on_treeView_doubleClicked(index);
}
