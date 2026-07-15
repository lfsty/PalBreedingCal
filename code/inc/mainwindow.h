#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

struct BreedingData;

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

private:
    // 内部接口，用于更新界面
    void updateBreedingView(const QVector<BreedingData>& breedingList);

private:
    Ui::MainWindow* ui;
};

#endif  // MAINWINDOW_H
