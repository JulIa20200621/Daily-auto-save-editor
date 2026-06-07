#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    //声明一个私有槽函数
    bool handleSaveAction();
    void handleClearAction();
    bool maybeSave();//判断文件是否发生改动
    bool saveFile(const QString &fileName); // 核心：专门负责写入的函数
    void openSettingsDialog();




public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QString m_currentFilePath; // 用来存当前打开的文件路径
};
#endif // MAINWINDOW_H
