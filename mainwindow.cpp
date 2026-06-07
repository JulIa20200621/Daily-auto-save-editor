#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include "storagesettingdialog.h" // 记得引入弹窗的头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //这里调用了save，然后把ui和这里的代码联系上了
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::handleSaveAction);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::handleClearAction);

    // 假设你在 UI 设计里创建的动作名字叫 actionSettings
    // 这一行非常关键，否则你点了菜单栏也不会有反应
    connect(ui->actionStoragePathSetting, &QAction::triggered, this, &MainWindow::openSettingsDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::handleSaveAction(){
    //获取保存文件的位置
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "文本文件 (*.txt);;所有文件 (*.*)");

    //如果取消了，直接返回empty
    if (fileName.isEmpty()) {
        return false;
    }

    //创建文件
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 如果打开失败，弹窗警告并结束
        QMessageBox::warning(this, "错误", "无法打开并写入该文件");
        return false;
    }

    //写入ui里面的文字
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();


    //保存并提示
    file.close(); // 显式关闭文件是一个好习惯
    ui->statusbar->showMessage("文件保存成功！", 3000); // 在状态栏显示 3 秒提示


    //保存进列表
    // m_savedFiles.prepend(fileName);

    //重复保存去掉重复路径
    // m_savedFiles.removeDuplicates();

    //更新显示列表
    // updateSavedFilesLayout();

    return true;

}



void MainWindow::handleClearAction(){
    //确认的提示
    // int ret = QMessageBox::warning(this, "警告", "确定要清空文本吗？未保存的内容将丢失。", QMessageBox::Yes | QMessageBox::No);

    //改为调用方法确认是否要清除
    if (!maybeSave()) {
        return;
    }

    //清楚的代码
    ui->textEdit->clear();

    // 清空后，别忘了标记为“未修改”，否则下一次关闭时还会弹窗
    ui->textEdit->document()->setModified(false);

}


bool MainWindow::maybeSave() {
    if (!ui->textEdit->document()->isModified()) {
        return true;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "提示",
                               "当前内容尚未保存，是否保存？",
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    // 修复点：明确处理每一种情况
    if (ret == QMessageBox::Save) {
        if (m_currentFilePath.isEmpty()) {
            return handleSaveAction(); // 这里会调用另存为
        } else {
            return saveFile(m_currentFilePath); // 这里直接保存
        }
    } else if (ret == QMessageBox::Cancel) {
        // 关键修复：点击取消，必须返回 false，终止后续的操作（如清空或关闭）
        return false;
    }

    // 如果是 Discard（放弃保存），自然会执行到这里，返回 true，表示允许执行清空操作
    return true;
}



bool MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开并写入该文件");
        return false; // 保存失败
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    // 更新当前文件路径
    m_currentFilePath = fileName;

    // 关键：告诉Qt“现在文件已经保存好了，不是被修改过的状态”
    // 这样下次就不会再弹窗烦你了！
    ui->textEdit->document()->setModified(false);

    ui->statusbar->showMessage("文件保存成功！", 3000);
    return true; // 保存成功
}

//加入设置存储路线的弹窗设定
void MainWindow::openSettingsDialog() {
    StorageSettingDialog dlg(this); // 创建弹窗实例
    if (dlg.exec() == QDialog::Accepted) {
        // 弹窗关闭后，如果用户点了“确定”，你可以在这里刷新主窗口的状态
        ui->statusbar->showMessage("设置已更新", 2000);
    }
}




