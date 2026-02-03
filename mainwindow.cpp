#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QToolTip> //->pop up message Qmessage box blocks UI sometimes so i used this.
#include <QCursor> //-> This show tooltip near mouse pointer.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable convert button initially(“I disable actions until user input becomes valid.”)
    // User should not convert without valid input
           ui->pushButton->setEnabled(false);



    //when in line edit we write any number and then press enter this signal and slot
    //takes place and user dont have to convert it rather than it can press enter.
    connect(ui->lineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::on_pushButton_clicked);


    // Enable convert button only when input is a valid number
    connect(ui->lineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text) /* here the lambda capture means:
         "I can access Ui elements eg ui->pushButton inside the block, without [=] , compiler will complain.
        &text = current text inside QLineEdit
        Passed by reference -> efficient
        const -> we promise not to modify it
         */
    {
        bool ok; //ok is true if it fails then its false.
        text.toDouble(&ok); // here it checks what user writes is it covertable like a num 21 is convertable and "acb" is not.

        // Button enabled only if conversion is possible
        ui->pushButton->setEnabled(ok);
    });



    //Initialize theme
    QSettings settings("MyCompany", "TempConverter");

    // Restore theme
    isDarkMode = settings.value("darkMode", true).toBool();
    if (isDarkMode)
        applyDarkTheme();
    else
        applyLightTheme();

    // Restore history
     QStringList historyList = settings.value("history").toStringList();
     ui->listWidget->addItems(historyList);


    this->setStyleSheet(
    "QMainWindow { background-color:#2b0037; }"
    "QLineEdit { background:#f3e5f5; color:#2b0037; border:2px solid #ba68c8; border-radius:10px; padding:10px; font-size:18px; }"
    "QLineEdit:focus { border:2px solid #9c27b0; }"
    "QPushButton { background-color:#9c27b0; color:white; font-size:18px; padding:10px; border-radius:12px; }"
    "QPushButton:hover { background-color:#ba68c8; }"
    "QLabel { color:#f3e5f5; }"
    );



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

        QString inputText = ui->lineEdit->text();

        if (inputText.isEmpty())
        {
            ui->label->setText("<p style='color:red;'> Please enter a value</p>");
            return;
        }

        bool ok;
        double inputTemp = inputText.toDouble(&ok);

        if (!ok)
        {
            ui->label->setText("<p style='color:red;'> Invalid number</p>");
            return;
        }

        double result = 0.0;
        double tempInCelsius = 0.0;
        QString inputUnit, outputUnit;

        int index = ui->comboBox->currentIndex();

        switch (index)
        {
            case 0: // C -> F
                result = (inputTemp * 9.0 / 5.0) + 32;
                tempInCelsius = inputTemp;
                inputUnit = "C"; outputUnit = "F";
                break;

            case 1: // F -> C
                result = (inputTemp - 32) * 5.0 / 9.0;
                tempInCelsius = result;
                inputUnit = "F"; outputUnit = "C";
                break;

            case 2: // C -> K
                result = inputTemp + 273.15;
                tempInCelsius = inputTemp;
                inputUnit = "C"; outputUnit = "K";
                break;

            case 3: // K -> C
                if (inputTemp < 0)
                {
                    ui->label->setText("<p style='color:red;'> Kelvin cannot be negative</p>");
                    return;
                }
                result = inputTemp - 273.15;
                tempInCelsius = result;
                inputUnit = "K"; outputUnit = "C";
                break;

            case 4: // F -> K
                result = (inputTemp - 32) * 5.0 / 9.0 + 273.15;
                tempInCelsius = (inputTemp - 32) * 5.0 / 9.0;
                inputUnit = "F"; outputUnit = "K";
                break;

            case 5: // K -> F
                if (inputTemp < 0)
                {
                    ui->label->setText("<p style='color:red;'> Kelvin cannot be negative</p>");
                    return;
                }
                result = (inputTemp - 273.15) * 9.0 / 5.0 + 32;
                tempInCelsius = inputTemp - 273.15;
                inputUnit = "K"; outputUnit = "F";
                break;
        }

        //  Get dynamic color
        QString color = getColorForTemperature(tempInCelsius);

        //  Final output
        ui->label->setText(
            "<p style='font-size:22px; color:" + color + ";'>" +
            QString::number(result, 'f', 2) + " " + outputUnit +
            "</p>"
        );

        animateResultLabel();

        // Add to history
        QString historyText =
                QString::number(inputTemp, 'f', 2) + " " + inputUnit +
                " -> " +
                QString::number(result, 'f', 2) + " " + outputUnit;

        ui->listWidget->addItem(historyText);
        animateHistoryList();

        // Enable clear button after successful conversion
        ui->clearButton->setEnabled(true);

    }






void MainWindow::on_comboBox_currentIndexChanged(int index)
{
//    if (index == 0)
//            ui->lineEdit->setPlaceholderText("Enter Celsius");
//        else
//            ui->lineEdit->setPlaceholderText("Enter Fahrenheit");

    switch(index)
    {
        case 0: ui->lineEdit->setPlaceholderText("Enter Celsius"); break;
        case 1: ui->lineEdit->setPlaceholderText("Enter Fahrenheit"); break;
        case 2: ui->lineEdit->setPlaceholderText("Enter Celsius"); break;
        case 3: ui->lineEdit->setPlaceholderText("Enter Kelvin"); break;
        case 4: ui->lineEdit->setPlaceholderText("Enter Fahrenheit"); break;
        case 5: ui->lineEdit->setPlaceholderText("Enter Kelvin"); break;
    }


}


void MainWindow::on_clearButton_clicked()
{
    // Clear input
        ui->lineEdit->clear();

        // Reset result label
        ui->label->setText(
            "<p style='color:white; font-size:18px;'>Result</p>"
        );

        // Clear history
        ui->listWidget->clear();

        // Set cursor back to input
        ui->lineEdit->setFocus();

        /*
      ****** This was my Old way to Clear Things *****

        Disable Clear button if nothing to clear
        ui->clearButton->setEnabled(false);

        Enable after conversion
        ui->clearButton->setEnabled(true);

        Disable again inside clear slot
        ui->clearButton->setEnabled(false);

        */

        /*           NEW WAY

         Clear button should be disabled initially
        It will be enabled only after first successful conversion
        WHY THE NEW WAY?
        -> Because it enables when data exists,and disables when data is gone.
        */
        ui->clearButton->setEnabled(false);


}


void MainWindow::on_themeButton_clicked()
{
//    if (isDarkMode)
//            applyLightTheme();
//        else
//            applyDarkTheme();

//        isDarkMode = !isDarkMode;

    animateThemeSwitch();
       isDarkMode = !isDarkMode;

}
void MainWindow::applyDarkTheme()
{
    this->setStyleSheet(
        "QMainWindow { background-color:#121212; }"

        "QLineEdit {"
        " background:#1e1e1e;"
        " color:white;"
        " border:2px solid #555;"
        " border-radius:8px;"
        " padding:8px;"
        " font-size:16px;"
        "}"

        "QPushButton {"
        " background:#333;"
        " color:white;"
        " border-radius:10px;"
        " padding:8px;"
        " font-size:16px;"
        "}"

        "QPushButton:hover { background:#444; }"

        "QLabel { color:white; }"
        "QListWidget { background:#1e1e1e; color:white; }"
        "QComboBox { background:#1e1e1e; color:white; }"
    );

    ui->themeButton->setText("☀️ Light Mode");
}

void MainWindow::applyLightTheme()
{
    this->setStyleSheet(
        "QMainWindow { background-color:#f0fff4; }"

        "QLineEdit {"
        " background:white;"
        " color:black;"
        " border:2px solid #bbb;"
        " border-radius:8px;"
        " padding:8px;"
        " font-size:16px;"
        "}"

        "QPushButton {"
        " background:#1976d2;"
        " color:white;"
        " border-radius:10px;"
        " padding:8px;"
        " font-size:16px;"
        "}"

        "QPushButton:hover { background:#1e88e5; }"

        "QLabel { color:black; }"
        "QListWidget { background:white; color:black; }"
        "QComboBox { background:white; color:black; }"
    );

    ui->themeButton->setText("🌙 Dark Mode");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("MyCompany", "TempConverter");

    // Save theme
    settings.setValue("darkMode", isDarkMode);

    // Save history
    QStringList historyList;
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        historyList << ui->listWidget->item(i)->text();
    }
    settings.setValue("history", historyList);

    event->accept();
}

    //Fade-In Animation for Result Label
    //I have animated the label every time result changes.

void MainWindow::animateResultLabel()
{
    // Create opacity effect
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->label);
    ui->label->setGraphicsEffect(effect);

    // Animation
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(400);          // milliseconds
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::animateHistoryList()
{
    QRect startRect = ui->listWidget->geometry();
    QRect endRect = startRect;

    startRect.moveLeft(startRect.left() + 40); // slide from right

    QPropertyAnimation *animation =
        new QPropertyAnimation(ui->listWidget, "geometry");

    animation->setDuration(300);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QString MainWindow::getColorForTemperature(double tempInCelsius)
{
    if (tempInCelsius <= 10)
        return "#4fc3f7";   // Blue (Cold)
    else if (tempInCelsius <= 30)
        return "#66bb6a";   // Green (Normal)
    else
        return "#ef5350";   // Red (Hot)
}

void MainWindow::animateThemeSwitch()
{
    // Fade OUT
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    // Fade IN
    QPropertyAnimation *fadeIn = new QPropertyAnimation(this, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    // When fade-out finishes → switch theme → fade-in
    connect(fadeOut, &QPropertyAnimation::finished, this, [=]() {
        if (isDarkMode)
            applyLightTheme();
        else
            applyDarkTheme();

        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_copyButton_clicked()
{
    // Get text from result label (strip HTML)
    QString resultText = ui->label->text();
    resultText.remove(QRegularExpression("<[^>]*>"));

    // Copy to clipboard
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(resultText);

//    // Optional user feedback
//    ui->statusbar->showMessage("Result copied to clipboard", 2000);

    // Show small popup near mouse cursor
        QToolTip::showText(QCursor::pos(), "Copied!");

}
