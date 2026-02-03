#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
private:
    void animateHistoryList();
    void animateResultLabel();
    void animateThemeSwitch();
private:
    QString getColorForTemperature(double tempInCelsius);


private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_clearButton_clicked();

    void on_themeButton_clicked();

    void on_copyButton_clicked();


private:
    Ui::MainWindow *ui;
    bool isDarkMode;
    void applyDarkTheme();
    void applyLightTheme();
};
#endif // MAINWINDOW_H
