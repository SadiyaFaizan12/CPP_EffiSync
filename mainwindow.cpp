#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCheckBox>
#include <QTimer>
#include<QDateTime>
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MPlayer = new QMediaPlayer();
    audioOutput =new QAudioOutput();
    MPlayer->setAudioOutput(audioOutput);

    ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->pushButton_nextSong->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pushButton_previousSong->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));



    connect(MPlayer,&QMediaPlayer::durationChanged,this, &MainWindow::durationChanged);
    connect(MPlayer,&QMediaPlayer::positionChanged,this, &MainWindow::positionChanged);

    ui->horizontalSlider->setRange(0,MPlayer->duration()/1000);
    ui->groupBox_2->hide();
    QTimer* timer = new QTimer(this); //to create a timer
    connect(timer, &QTimer::timeout, this, &MainWindow::updateLabel); //to connect the timer to update label slot
    timer->start(10000); // Update every 10 seconds
    ui->label_15->setText("Weather: ");

    fetchWeatherData();
    dateTimeLabel = ui->label1;

    if(dateTimeLabel) {
        // Create a timer to update the time every second
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
        timer->start(1000); // Update every 1000 milliseconds (1 second)

        // Update the time immediately
        updateTime();
    } else {
        qDebug() << "Label not found!";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QHash<QDate, QVector<QVector<QString>>> myHash; // hash with date as keys and nested vec as value for reminders
QVector<QString> ToDo; //dynamic array to store tasks


void MainWindow::on_push_Button_clicked() // to add reminders
{
    ui->remin->clear();

    QString title = ui->title->text();
    QString description = ui->desc->text();
    QTime time = ui->t1->time();
    QString strTime=time.toString("hh:mm:ss");
    QString email = ui->email->text();
    QDate date = ui->calendarWidget->selectedDate();
    QString formattedDate = date.toString("yyyy-MM-dd");
    QString dataString;
    dataString = QString();
    QVector<QString> dataArray; //vector with string of reminder details

    dataArray.append(title);
    dataArray.append(description);
    dataArray.append(strTime);
    dataArray.append(email);

    QVector<QVector<QString>> dateArray; // nested vector with string inside inner vector


    dateArray.append(dataArray);

    myHash[date]=dateArray;

    QVector<QVector<QString>> value = myHash.value(date); //value stores vector with reminders of a date

    for (const QVector<QString> &inner:value) //
    {
        dataString+="Date : "+formattedDate+"\n";
        dataString+="Title : "+inner[0]+"\n";
        dataString+="Description : "+inner[1]+"\n";
        dataString+="Time : "+inner[2]+"\n";
        dataString+="Email : "+inner[3]+"\n\n";
    }
    ui->remin->setText(dataString);
    ui->title->clear();
    ui->desc->clear();
    ui->email->clear();
    ui->lineEdit_6->clear();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date) //to show reminders of date selected on calender
{
    ui->remin->clear();


    QString dataString;
    QVector<QVector<QString>> value = myHash.value(date);
    QString formattedDate = date.toString("yyyy-MM-dd");
    if (!value.isEmpty())
        {
            for (const QVector<QString> &inner:value)
            {
                dataString+="Date : "+formattedDate+"\n";
                dataString+="Title : "+inner[0]+"\n";
                dataString+="Description : "+inner[1]+"\n";
                dataString+="Time : "+inner[2]+"\n";
                dataString+="Email : "+inner[3]+"\n\n";
            }
            ui->remin->setText(dataString); // to show reminders of the selected date
        }
}

void MainWindow::on_pushButton_6_clicked() // to search reminder
{
    ui->remin->clear();

    int count=0; //to count number of matched reminders
    QString reminder; //to store reminder
    QString title = ui->lineEdit_6->text(); //take title of reminder
    QHash<QDate, QVector<QVector<QString>>>::const_iterator it; //to create a Qhash iterator it, :: is used for scope resolution
    for (it = myHash.constBegin(); it != myHash.constEnd(); ++it) //to iterate thru all dates
    {
        QDate key = it.key(); //get the date
        QString formattedDate = key.toString("yyyy-MM-dd"); // Define the format you want
        QVector<QVector<QString>> value = it.value(); //get all the reminders for that date
        for (const QVector<QString> &inner:value) //to iterate thru each reminder of a date 'key', &->reference to inner and does not make copy
        {
            QString title_1=inner[0];
            if (title_1==title)
            {
                count+=1;
                reminder+="Date : "+formattedDate+"\n";
                reminder+="Title : "+inner[0]+"\n";
                reminder+="Description : "+inner[1]+"\n";
                reminder+="Time : "+inner[2]+"\n";
                reminder+="Email : "+inner[3]+"\n\n";
            }
        }
    }
    if (count>0)
    {
        ui->remin->setText(reminder);
    }
    else
    {
        ui->remin->setText("No reminders found");
    }
    //ui->lineEdit_6->clear();
}

void MainWindow::on_pushButton_2_clicked() // to delete reminder
{
    ui->remin->clear();
    QString title = ui->lineEdit_6->text(); // title of reminder to delete
    int count = 0; // number of deleted reminders

    QHash<QDate, QVector<QVector<QString>>>::iterator it_del; // Use a non-const iterator to modify the hash
    for (it_del = myHash.begin(); it_del != myHash.end(); /* no need to increment here */) // Iterate through all dates
    {
        QVector<QVector<QString>>& value = it_del.value(); // Get all the reminders for that date by reference
        QVector<QVector<QString>>::iterator innerIt = value.begin(); // iterator for value vector
        while (innerIt != value.end()) // Iterate through each reminder of a date 'key'
        {
            QString title_1 = (*innerIt)[0];
            if (title_1 == title)
            {
                innerIt = value.erase(innerIt); // Remove the inner vector if title matches
                ++count;
            }
            else
            {
                ++innerIt;
            }
        }
        // Remove the date entry if all reminders for that date were deleted
        if (value.isEmpty())
        {
            it_del = myHash.erase(it_del);
        }
        else
        {
            ++it_del; // Move to the next date
        }
    }

    if (count > 0)
    {
        QString fin = QString::number(count);
        ui->remin->setText(fin + " reminder(s) deleted.");
    }
    else
    {
        ui->remin->setText("No reminders found.");
    }
    ui->lineEdit_6->clear();
}

void MainWindow::on_pushButton_3_clicked() //to open edit reminder window
{
    ui->groupBox_2->show();
    ui->label_15->move(1230,0);
    ui->label_16->resize(919,60);
    this->resize(1500, 740);
    int count=0;
    ui->remin->clear();
    QString title = ui->lineEdit_6->text(); //take title of reminder
    QHash<QDate, QVector<QVector<QString>>>::const_iterator it; //to create a Qhash iterator it
    for (it = myHash.constBegin(); it != myHash.constEnd(); ++it) //to iterate thru all dates
    {
        QDate key = it.key(); //get the date
        QVector<QVector<QString>> value = it.value(); //get all the reminders for that date
        for (const QVector<QString> &inner:value) //to iterate thru each reminder of a date 'key'
        {
            QString title_1=inner[0];
            if (title_1==title)
            {
                count+=1;
                ui->lineEdit->setText(inner[0]);//title
                ui->lineEdit_3->setText(inner[1]);//desc
                ui->timeEdit_3->setTime(QTime::fromString(inner[2])); //time
                ui->lineEdit_4->setText(inner[3]);//email
                break;
            }
        }
        if (count==1)
        {
            break;
        }
    }
}

void MainWindow::on_pushButton_5_clicked() //to update reminder
{
    ui->remin->clear();
    QString title = ui->lineEdit_6->text(); // title of reminder to delete
    //int count = 0; // number of deleted reminders

    QHash<QDate, QVector<QVector<QString>>>::iterator it_del; // Use a non-const iterator to modify the hash
    for (it_del = myHash.begin(); it_del != myHash.end(); /* no need to increment here */) // Iterate through all dates
    {
        QVector<QVector<QString>>& value = it_del.value(); // Get all the reminders for that date by reference
        QVector<QVector<QString>>::iterator innerIt = value.begin(); // iterator for value vector
        while (innerIt != value.end()) // Iterate through each reminder of a date 'key'
        {
            QString title_1 = (*innerIt)[0];
            if (title_1 == title)
            {
                innerIt = value.erase(innerIt); // Remove the inner vector if title matches
                //++count;
            }
            else
            {
                ++innerIt;
            }
        }
        // Remove the date entry if all reminders for that date were deleted
        if (value.isEmpty())
        {
            it_del = myHash.erase(it_del);
        }
        else
        {
            ++it_del; // Move to the next date
        }
    }

    ui->remin->clear();

    QString title2 = ui->lineEdit->text();
    QString description2 = ui->lineEdit_3->text();
    QTime time2 = ui->timeEdit_3->time();
    QString strTime=time2.toString("hh:mm:ss");
    QString email2 = ui->lineEdit_4->text();
    QDate date = ui->calendarWidget->selectedDate();
    QString formattedDate = date.toString("yyyy-MM-dd");
    QString dataString;
    dataString = QString();
    QVector<QString> dataArray; //vector with string of reminder details

    dataArray.append(title2);
    dataArray.append(description2);
    dataArray.append(strTime);
    dataArray.append(email2);

    QVector<QVector<QString>> dateArray; // nested vector with string inside inner vector


    dateArray.append(dataArray);

    myHash[date]=dateArray;

    QVector<QVector<QString>> value = myHash.value(date); //value stores vector with reminders of a date

    for (const QVector<QString> &inner:value) //
    {
        dataString+="Date : "+formattedDate+"\n";
        dataString+="Title : "+inner[0]+"\n";
        dataString+="Description : "+inner[1]+"\n";
        dataString+="Time : "+inner[2]+"\n";
        dataString+="Email : "+inner[3]+"\n\n";
    }
    ui->label_11->setText(dataString);
    ui->remin->setText("Updated successfully");
}

void MainWindow::on_pushButton_4_clicked() //cancel update
{
    ui->groupBox_2->hide();
    ui->label_15->move(960,0);
    ui->label_16->resize(649,60);
    this->resize(1230, 740);
}

void MainWindow::on_pushButton_7_clicked() //close after update
{
    ui->groupBox_2->hide();
     ui->label_15->move(960,0);
    ui->label_16->resize(649,60);
    this->resize(1230, 740);
    ui->label_11->clear();
}

void MainWindow::on_pushButton_clicked() // to add task to To-Do list
{
    QString task=ui->lineEdit_7->text();
    ToDo.append(task);
    QCheckBox * newCheckbox = new QCheckBox(task, this);
    newCheckbox->setChecked(false);
    ui->verticalLayout->addWidget(newCheckbox);
    ui->lineEdit_7->clear();
}

void MainWindow::on_pushButton_8_clicked() // to delete task from To-Do list
{
    QString taskToDelete = ui->lineEdit_7->text(); // Get the text input for the task to delete

    // Iterate through the widgets in the vertical layout
    for (int i = 0; i < ui->verticalLayout->count(); ++i)
    {
        QCheckBox* checkbox = qobject_cast<QCheckBox*>(ui->verticalLayout->itemAt(i)->widget());
        if (checkbox && checkbox->text() == taskToDelete)
        {
            // If a checkbox with matching text is found, remove it from the layout and delete it
            ui->verticalLayout->removeWidget(checkbox);
            delete checkbox;
            break; // Exit the loop after deleting the checkbox
        }
    }
    ui->lineEdit_7->clear();
}

void MainWindow::updateLabel() //to show quotes dynamically with timer
{
    static int currentIndex = 0;
    const QStringList textArray = {
                "Believe you can, and you're halfway there. - Theodore Roosevelt",
                "Don't watch the clock; do what it does. Keep going. - Sam Levenson",
                "Success is not final, failure is not fatal: It is the courage to continue that counts. - Winston Churchill",
                "Hard work beats talent when talent doesn't work hard. - Tim Notke",
                "The only way to do great work is to love what you do. - Steve Jobs",
                "You are never too old to set another goal or to dream a new dream. - C.S. Lewis",
                "The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt",
                "The only limit to our realization of tomorrow will be our doubts of today. - Franklin D. Roosevelt",
                "What lies behind us and what lies before us are tiny matters compared to what lies within us. - Ralph Waldo Emerson",
                "Your time is limited, so don't waste it living someone else's life. - Steve Jobs",
                "It always seems impossible until it's done. - Nelson Mandela",
                "Don't be pushed around by the fears in your mind. Be led by the dreams in your heart. - Roy T. Bennett",
                "The harder you work for something, the greater you'll feel when you achieve it. - Unknown",
                "You miss 100% of the shots you don't take. - Wayne Gretzky",
                "The only way to do great work is to love what you do. - Steve Jobs",
                "Success is not the key to happiness. Happiness is the key to success. If you love what you are doing, you will be successful. - Albert Schweitzer",
                "The biggest risk is not taking any risk. In a world that is changing quickly, the only strategy that is guaranteed to fail is not taking risks. - Mark Zuckerberg",
                "The only person you are destined to become is the person you decide to be. - Ralph Waldo Emerson",
                "You have within you the strength, the patience, and the passion to reach for the stars to change the world. - Harriet Tubman",
                "Believe in yourself and all that you are. Know that there is something inside you that is greater than any obstacle. - Christian D. Larson"
            };


    // Update the label text with the current string from the array
    ui->label_5->setText(textArray[currentIndex]);

    // Increment the index for the next iteration
    currentIndex = (currentIndex + 1) % textArray.size();
}
void MainWindow::durationChanged(qint64 duration)
{
    Mduration= duration/1000;
    ui->horizontalSlider->setMaximum(Mduration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if(!ui->horizontalSlider->isSliderDown())
    {
        ui->horizontalSlider->setValue(progress/1000);
    }
    updateduration(progress/1000);
}
QList<QString> playlist;
int currentSongIndex = -1;
void MainWindow::loadsong(const QString &folderPath)
{
    QDir dir(folderPath);
    QStringList filters;
    filters << "*.mp3"; // You can add more file extensions if needed
    dir.setNameFilters(filters);

    QStringList fileList = dir.entryList(QDir::Files);
    for (const QString& file : fileList) {
        QString filePath = dir.absoluteFilePath(file);
        playlist.append(filePath);
    }

    if (!playlist.isEmpty()) {
        currentSongIndex = 0; // Start playing the first song
        playCurrentSong();
    }
}

void MainWindow::playCurrentSong()
{
    if (currentSongIndex >= 0 && currentSongIndex < playlist.size()) {
        QString currentSong = playlist[currentSongIndex];
        MPlayer->setSource(QUrl::fromLocalFile(currentSong));

        QFileInfo fileInfo(currentSong); //to print current song details
        ui->label_12->setText(fileInfo.fileName());
    }
    else {
        // Invalid currentSongIndex, handle this case (e.g., show a message)
        QMessageBox::critical(this, tr("Error"), tr("Invalid song index."));
    }
}

void MainWindow::playPreviousSong()
{
    if (currentSongIndex > 0 && currentSongIndex < playlist.size()) {
        currentSongIndex--;
        playCurrentSong();
    }
    else {
        // Already at the beginning of the playlist, handle this case (e.g., show a message)
        QMessageBox::information(this, tr("Start of Playlist"), tr("Already at the beginning of the playlist."));
    }
}




void MainWindow::on_pushButton_Volume_clicked()
{
    if(IS_Muted==false)
    {
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        IS_Muted=true;
        audioOutput->setMuted(true);
    }
    else
    {
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        IS_Muted=false;
        audioOutput->setMuted(false);

    }
}


/*
void MainWindow::on_actionOpen_File_triggered()
{

    //QString FileName = QFileDialog::getOpenFileName(this,tr("Select Audio File"),"",tr("MP3 Files(*.mp3)"));
    //MPlayer->setSource(QUrl(FileName));

    //QFileInfo fileinfo(FileName);
    //ui->label->setText(fileinfo.fileName());


    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"), "", QFileDialog::ShowDirsOnly);

    if (!folderPath.isEmpty()) {
        loadsong(folderPath);
    }
}
*/

void MainWindow::on_pushButton_Stop_clicked()
{
    MPlayer->stop();
}


void MainWindow::on_pushButton_Pause_clicked()
{
    MPlayer->pause();
}


void MainWindow::on_pushButton_Play_clicked()
{
    MPlayer->play();
}


void MainWindow::updateduration(qint64 duration)
{
    QString timestr;
    if(duration || Mduration)
    {
        QTime CurrentTime((duration / 3600) % 60,(duration/60)% 60,duration % 60,(duration * 1000) % 1000);
        QTime totalTime((Mduration / 3600) % 60,(Mduration/60)% 60,Mduration % 60,(Mduration * 1000) % 1000);
        QString format = "mm:ss";
        if(Mduration >3600)
            format ="hh:mm:ss";
        ui->label_14->setText(CurrentTime.toString(format));
        ui->label_13->setText(totalTime.toString(format));

    }
}






void MainWindow::on_pushButton_nextSong_clicked()
{
    if (currentSongIndex < playlist.size() - 1) {
        currentSongIndex++;
        playCurrentSong();
    }
    else {
        // No more songs in the playlist, handle this case (e.g., show a message)
        QMessageBox::information(this, tr("End of Playlist"), tr("No more songs in the playlist."));
    }
}


void MainWindow::on_pushButton_previousSong_clicked()
{
    playPreviousSong();
}



void MainWindow::on_actionOpen_Audio_File_triggered()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"), "", QFileDialog::ShowDirsOnly);

    if (!folderPath.isEmpty()) {
        loadsong(folderPath);
    }
}
void MainWindow::fetchWeatherData()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onWeatherDataReceived);

    // Replace "YOUR_API_KEY" with your OpenWeatherMap API key
    QString apiKey = "Put_Your_API_Key";
    QString city = "Noida";
    QString urlStr = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey;


    QUrl url(urlStr);
    QNetworkRequest request(url);

    manager->get(request);
}

void MainWindow::onWeatherDataReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (!jsonResponse.isNull()) {
            QJsonObject jsonObj = jsonResponse.object();

            if (jsonObj.contains("main") && jsonObj["main"].isObject()) {
                double temperatureKelvin = jsonObj["main"].toObject()["temp"].toDouble();
                double temperatureCelsius = temperatureKelvin - 273.15;

                QJsonValue weatherValue = jsonObj["weather"].toArray()[0].toObject()["description"];
                QString weatherDesc = weatherValue.toString();

                QLabel *weatherLabel = ui->label_15;
                if (weatherLabel) {
                    QString displayText = "Temperature: " + QString::number(temperatureCelsius, 'f', 2) + "°C\n";
                                          displayText += "Weather: " + weatherDesc;
                    weatherLabel->setText(displayText);
                }

                // Display the weather description on the label

            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void MainWindow::updateTime()
{
    if(dateTimeLabel) {
        // Get current date and time
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // Display date and time in a specific format
        QString dateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        // Update the label with the current date and time
        dateTimeLabel->setText(dateTimeStr);
    }
}

QVector<QString> quickNotes; //dynamic array to store quick notes



void MainWindow::on_pushButton_9_clicked()
{
    QString note = ui->plainTextEdit->toPlainText(); //stores the note
    quickNotes.append(note);
    /*QString finalText;
    for (const QString& note : quickNotes)
    {
        finalText.append(note).append("\n");
    }*/

    // Display the final text in the QPlainTextEdit
    ui->plainTextEdit->setPlainText(note);
}


void MainWindow::on_pushButton_10_clicked()
{
    ui->plainTextEdit->setPlainText("");
}


void MainWindow::on_pushButton_11_clicked()
{
    QString currentText = ui->plainTextEdit->toPlainText();

    if (currentText.isEmpty())
    {
        // If the plainTextEdit is clear, display the last element of quickNotes
        if (!quickNotes.isEmpty())
        {
            ui->plainTextEdit->setPlainText(quickNotes.last());
        }
        else
        {
            ui->plainTextEdit->setPlainText("No notes available.");
        }
    }
    else
    {
        // If there is text in plainTextEdit, find the current text in quickNotes and display the previous text from the list
        int index = quickNotes.indexOf(currentText);
        if (index > 0)
        {
            ui->plainTextEdit->setPlainText(quickNotes[index - 1]);
        }
        else
        {
            ui->plainTextEdit->setPlainText("No previous note available.");
        }
    }
}


void MainWindow::on_pushButton_12_clicked()
{
QString currentText = ui->plainTextEdit->toPlainText();

    if (currentText.isEmpty())
    {
        // If the plainTextEdit is clear, display the last element of quickNotes
        if (!quickNotes.isEmpty())
        {
            ui->plainTextEdit->setPlainText(quickNotes.last());
        }
        else
        {
            ui->plainTextEdit->setPlainText("No notes available.");
        }
    }
    else
    {
        // If there is text in plainTextEdit, find the current text in quickNotes and display the next text from the list
        int index = quickNotes.indexOf(currentText);
        if (index < quickNotes.size()-1)
        {
            ui->plainTextEdit->setPlainText(quickNotes[index + 1]);
        }
        else
        {
            ui->plainTextEdit->setPlainText("No new note available.");
        }
    }
}

