#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtMultimedia/QtMultimedia>
#include <QtCore>
#include <QtWidgets>
#include <QMediaPlayer>
#include<QAudioOutput>
#include<QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include<QJsonArray>
#include<QTimer>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_push_Button_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void updateLabel();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void loadsong(const QString& folderPath);
    void playCurrentSong();
    void playPreviousSong();

    void on_pushButton_Volume_clicked();


    void on_pushButton_Stop_clicked();

    void on_pushButton_Pause_clicked();

    void on_pushButton_Play_clicked();






    void on_pushButton_nextSong_clicked();

    void on_pushButton_previousSong_clicked();




    void on_actionOpen_Audio_File_triggered();


    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::MainWindow *ui;
    void updateduration(qint64 duration);

    bool IS_Muted = false;
    QMediaPlayer  *MPlayer;
    QAudioOutput  *audioOutput;
    qint64 Mduration;
    void fetchWeatherData();
    void onWeatherDataReceived(QNetworkReply *reply);
    void updateTime();
    QTimer *timer;
    QLabel *dateTimeLabel;

};
#endif // MAINWINDOW_H
