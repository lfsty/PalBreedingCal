#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BreedingModel;

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

protected slots:
    // 跟新接口，更新数据和刷新界面
    void updateBreedingList();
    void updateComboBox();

signals:
    // 内部信号，用于更新界面
    void requestUpdateBreedingView(const QVector<BreedingModel>& breedingList);

private:
    Ui::MainWindow* ui;
};

#endif  // MAINWINDOW_H
