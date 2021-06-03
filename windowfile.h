#ifndef WINDOWFILE_H
#define WINDOWFILE_H



#include <QDialog>
#include <QFileSystemModel>
#include "setting.h"

namespace Ui {
class WindowFile;
}

class WindowFile : public QDialog
{
    Q_OBJECT

public:
    explicit WindowFile(QWidget *parent = nullptr);
    ~WindowFile();

    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_treeView_clicked(const QModelIndex &index);

    bool FileOrDir;
    QString CheminComplet;


private slots:
    void on_ChangeLink_clicked();
    void on_ChoiceLink_textChanged(const QString &arg1);


private:
    Ui::WindowFile *ui;
    QFileSystemModel* dirmodel;
};

#endif // WINDOWFILE_H
