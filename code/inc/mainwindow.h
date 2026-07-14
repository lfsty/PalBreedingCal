#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCompleter>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QString>

#include <PalManager.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void updateComboAndList();

private:
    Ui::MainWindow* ui;
    QStandardItemModel* m_breedingModel = nullptr;
};

#endif  // MAINWINDOW_H
