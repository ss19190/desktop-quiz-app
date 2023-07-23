#include "user.h"
#include "quiz.h"
#include "./ui_quiz.h"
#include "question.h"
#include <array>

QString currentPath = QDir::currentPath() + "/../quiz2_0";

int Quiz::get_random_in_range(int from, int to)
{
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(from, to);
    return distribution(generator);
}

std::pair<QString, QString> Quiz::getInformation()
{
    QItemSelectionModel *selectionModelQuizes = listViewQuizes->selectionModel();
    QModelIndex selectedIndexQuiz = selectionModelQuizes->currentIndex();

    QItemSelectionModel *selectionModelUsers = listViewUsers->selectionModel();
    QModelIndex selectedIndexUser = selectionModelUsers->currentIndex();

    if (!selectedIndexQuiz.isValid())
    {
        if (!selectedIndexUser.isValid())
        {
            QMessageBox::information(this, "Error", "Please select a quiz and an user");
        }
        else
        {
            QMessageBox::information(this, "Error", "Please select a quiz");
        }
    }
    else if (!selectedIndexUser.isValid())
    {
        QMessageBox::information(this, "Error", "Please select an user");
    }
    else
    {

        QVariant itemDataQuiz = selectedIndexQuiz.data();
        QVariant itemDataUser = selectedIndexUser.data();

        QString selectedItemTextQuiz = itemDataQuiz.toString();
        QString selectedItemTextUser = itemDataUser.toString();

        std::pair<QString, QString> info = std::make_pair(selectedItemTextQuiz, selectedItemTextUser);

        qDebug() << "Selected Quiz: " << selectedItemTextQuiz;
        qDebug() << "Selected User: " << selectedItemTextUser;

        return info;
    }

    return std::pair<QString, QString>();
}

std::unordered_map<int, Question> Quiz::createQuiz(QString filePath)
{

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        std::unordered_map<int, Question> quiz;

        int counter = 1;

        if(ui -> ABCDbutton -> isChecked()){
            while (!in.atEnd())
            {
                Question4Ans wholeQuestion(in);

                quiz[counter] = wholeQuestion;
                counter++;
            }
        }

        if(ui -> buttonTrue -> isChecked()){
            while (!in.atEnd())
            {
                Question2Ans wholeQuestion(in);

                quiz[counter] = wholeQuestion;
                counter++;
            }
        }



        return quiz;
    }
    return std::unordered_map<int, Question>();
}

std::unordered_map<int, User> Quiz::createUsers()
{

    QFile file(currentPath + "/users.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        std::unordered_map<int, User> lusers;
        std::string name;

        while (!in.atEnd())
        {
            User user(in);
            lusers[user.get_id()] = user;
        }

        file.close();
        users = lusers;
        return lusers;
    }
    users = std::unordered_map<int, User>();
    return std::unordered_map<int, User>();
}

void Quiz::loadFileToListView(QListView *listView, QString filePath)
{

    QStringList dataList;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line;
        while (!in.atEnd())
        {
            line = in.readLine();
            dataList << line;
            line = in.readLine();
            while (!line.isEmpty())
            {
                line = in.readLine();
            }
        }
        file.close();
    }
    else
    {
        dataList << "aaaa";
    }

    QStringListModel *listModel = new QStringListModel(this);
    listModel->setStringList(dataList);
    listView->setModel(listModel);
    listView->update();
}

void Quiz::parseUsersToListView(QListView *listView, QString filePath)
{

    QStringList dataList;
    for (auto &user : users)
    {
        dataList << QString::fromStdString(user.second.get_name());
    }
    QStringListModel *listModel = new QStringListModel(this);
    listModel->setStringList(dataList);
    listView->setModel(listModel);
    listView->update();
}

void saveToFile(const QString &filePath, const QString &text)
{
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "\n\n"
            << text;
        file.close();
    }
}

void Quiz::changeUserScore(const QString &userName, const QString &quizName, int newScore)
{
    QFile file(currentPath + "/users.txt");
    QStringList lines;
    int usrId = 0;
    for (auto &u : users)
    {
        if (u.second.get_name() == userName.toStdString())
            usrId = u.first;
    }
    users[usrId].set_highscore(quizName.toStdString(), newScore);
    for (auto &user : users)
    {
        user.second.save_to_string_list(lines);
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "git?";
        QTextStream out(&file);
        for (const QString &line : lines)
        {
            out << line << "\n";
        }
    }
    else
    {
        qDebug() << "eh 2";
    }
    file.close();
}

void Quiz::runQuizABCD(){
    quizWidget->setTabEnabled(1, true);
    quizWidget->setCurrentIndex(1);
    quizWidget->widget(1)->setEnabled(true);
    std::pair<QString, QString> info = getInformation();

    username = ui->username;
    username->setText("User: " + info.second);

    QString path = currentPath + "/" + info.first + ".txt";
    std::unordered_map<int, Question> quiz = createQuiz(path);

    questionaLabel = ui->questionaLabel;
    questionaLabel->setWordWrap(true);
    QMargins padding(10, 10, 10, 10);
    questionaLabel->setAlignment(Qt::AlignCenter);
    questionaLabel->setContentsMargins(padding);
    buttonA = ui->answer1;
    buttonB = ui->answer2;
    buttonC = ui->answer3;
    buttonD = ui->answer4;
    buttonC -> setVisible(true);
    buttonD -> setVisible(true);
    questionCounter = ui->questionCounter;
    scoreLabel = ui->scoreLabel;

    int arr[20];
    int score = 0;
    int i = 0;

    // Generate random numbers and store them in the array
    for (int i = 0; i < 20; i++)
    {

        int randomNum = get_random_in_range(1, 30);

        // Check if the generated number is already in the array
        while (std::find(std::begin(arr), std::end(arr), randomNum) != std::end(arr))
        {
            randomNum = get_random_in_range(1, 30);
        }

        arr[i] = randomNum;
        QString question = quiz[randomNum].get_question();
        std::array<QString, 4> answers = quiz[randomNum].get_answers();
        QString answerA = answers[0];
        QString answerB = answers[1];
        QString answerC = answers[2];
        QString answerD = answers[3];
        QString correctAnswer = quiz[randomNum].get_correct_answer();
        questionaLabel->setText(question);
        buttonA->setText(answerA);
        buttonB->setText(answerB);
        buttonC->setText(answerC);
        buttonD->setText(answerD);
        questionCounter->setText("Question: " + QString::number(i + 1) + "/20");
        scoreLabel->setText("Score: " + QString::number(score));

        // Use a local event loop to wait for user input
        QEventLoop loop;

        // Create a lambda function to handle button clicks
        auto handleButtonClick = [this, &loop, &correctAnswer, &score](QPushButton *clickedButton)
        {
            // Disconnect the buttons to prevent multiple signals
            buttonA->disconnect();
            buttonB->disconnect();
            buttonC->disconnect();
            buttonD->disconnect();

            // Get the text of the clicked button
            QString clickedButtonText = clickedButton->text();
            if (clickedButtonText == correctAnswer)
                score++;
            // Quit the event loop to resume execution
            loop.quit();
        };

        // Connect the clicked signal of each button to the lambda function
        connect(buttonA, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonA); });
        connect(buttonB, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonB); });
        connect(buttonC, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonC); });
        connect(buttonD, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonD); });

        // Run the local event loop to wait for user input
        loop.exec();
    }

    changeUserScore(info.second, info.first, score);
    questionaLabel->setText("Quiz finished with a score: " + QString::number(score));
    quizWidget->setCurrentIndex(2);
    quizWidget->widget(1)->setEnabled(false);
    showHighscore(createUsers(), info.first);
}

void Quiz::runQuizTrueOrFalse(){
    quizWidget->setTabEnabled(1, true);
    quizWidget->setCurrentIndex(1);
    quizWidget->widget(1)->setEnabled(true);
    std::pair<QString, QString> info = getInformation();

    username = ui->username;
    username->setText("User: " + info.second);

    QString path = currentPath + "/" + info.first + "2.txt";
    std::unordered_map<int, Question> quiz = createQuiz(path);

    questionaLabel = ui->questionaLabel;
    questionaLabel->setWordWrap(true);
    QMargins padding(10, 10, 10, 10);
    questionaLabel->setAlignment(Qt::AlignCenter);
    questionaLabel->setContentsMargins(padding);
    buttonA = ui->answer1;
    buttonB = ui->answer2;
    buttonC = ui->answer3;
    buttonD = ui->answer4;
    buttonC -> setVisible(false);
    buttonD -> setVisible(false);
    questionCounter = ui->questionCounter;
    scoreLabel = ui->scoreLabel;

    int arr[20];
    int score = 0;
    int i = 0;

    buttonA->setText("True");
    buttonB->setText("False");

    // Generate random numbers and store them in the array
    for (int i = 0; i < 20; i++)
    {

        int randomNum = get_random_in_range(1, 30);

        // Check if the generated number is already in the array
        while (std::find(std::begin(arr), std::end(arr), randomNum) != std::end(arr))
        {
            randomNum = get_random_in_range(1, 30);
        }

        arr[i] = randomNum;
        QString question = quiz[randomNum].get_question();
        QString correctAnswer = quiz[randomNum].get_correct_answer();
        questionaLabel->setText(question);

        questionCounter->setText("Question: " + QString::number(i + 1) + "/20");
        scoreLabel->setText("Score: " + QString::number(score));

        // Use a local event loop to wait for user input
        QEventLoop loop;

        // Create a lambda function to handle button clicks
        auto handleButtonClick = [this, &loop, &correctAnswer, &score](QPushButton *clickedButton)
        {
            // Disconnect the buttons to prevent multiple signals
            buttonA->disconnect();
            buttonB->disconnect();
            buttonC->disconnect();
            buttonD->disconnect();

            // Get the text of the clicked button
            QString clickedButtonText = clickedButton->text();
            if (clickedButtonText == correctAnswer)
                score++;
            // Quit the event loop to resume execution
            loop.quit();
        };

        // Connect the clicked signal of each button to the lambda function
        connect(buttonA, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonA); });
        connect(buttonB, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonB); });
        connect(buttonC, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonC); });
        connect(buttonD, &QPushButton::clicked, this, [this, handleButtonClick]()
                { handleButtonClick(buttonD); });

        // Run the local event loop to wait for user input
        loop.exec();
    }

    changeUserScore(info.second, info.first, score);
    questionaLabel->setText("Quiz finished with a score: " + QString::number(score));
    quizWidget->setCurrentIndex(2);
    quizWidget->widget(1)->setEnabled(false);
    showHighscore(createUsers(), info.first);
}

void Quiz::startButtonClicked()
{

    if(ui -> ABCDbutton -> isChecked()){
        runQuizABCD();
    }

    if(ui -> buttonTrue -> isChecked()){
        runQuizTrueOrFalse();
    }

}

void Quiz::addUserButtonClicked()
{

    QString filePath = currentPath + "/users.txt";
    textEditUser = ui->addUserText;
    QString text = textEditUser->toPlainText();
    qDebug() << "eh213";
    User user(users.size(), text.toStdString());
    qDebug() << "eheh";
    users[user.get_id()] = user;
    qDebug() << "ehhhhehhheh";
    QFile file(currentPath + "/users.txt");
    QStringList lines;
    for (auto &user : users)
    {
        qDebug() << "user: " << QString::fromStdString(user.second.get_name());
        user.second.save_to_string_list(lines);
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "git?";
        QTextStream out(&file);
        for (const QString &line : lines)
        {
            out << line << "\n";
        }
    }
    else
    {
        qDebug() << "eh 2";
    }
    file.close();
    textEditUser->clear();
    parseUsersToListView(listViewUsers, filePath);
}

void Quiz::showHighscore(std::unordered_map<int, User> users, QString quiz)
{

    highscoreLabel = ui->highscoresLabel;
    highscoreLabel->setText("Highscores for quiz: " + quiz);

    QString ranking = "";
    for (const auto &pair : users)
    {
        User currentUser = pair.second;
        QString name = QString::fromStdString(currentUser.get_name());
        ranking += name;
        ranking += "\n";
        std::unordered_map<std::string, int> highscores = currentUser.get_highscores();
        auto it = highscores.find(quiz.toStdString());
        if (it != highscores.end())
        {
            ranking += QString::number(it->second);
            ranking += "\n";
        }
        else
        {
            ranking += "0\n";
        }

        ranking += "\n";
    }

    textEditHighscore->setText(ranking);
}

void Quiz::handleItemClicked(const QModelIndex &index)
{
    QVariant itemData = index.data(Qt::DisplayRole);
    if (itemData.isValid())
    {
        QString itemValue = itemData.toString();
        showHighscore(createUsers(), itemValue);
    }
}

void Quiz::seeHighscoreClicked()
{

    QItemSelectionModel *selectionModelHighscore = listViewHighscore->selectionModel();
    QModelIndex selectedIndexHighscore = selectionModelHighscore->currentIndex();

    if (!selectedIndexHighscore.isValid())
    {
        QMessageBox::information(this, "Error", "Please select a quiz");
    }
    else
    {
        QVariant itemDataHighscore = selectedIndexHighscore.data();

        QString selectedItemTextHighscore = itemDataHighscore.toString();

        showHighscore(createUsers(), selectedItemTextHighscore);
    }
}

Quiz::Quiz(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Quiz)
{
    createUsers();
    ui->setupUi(this);

    qDebug() << "path: " << currentPath;

    listModelUsers = new QStringListModel(this);
    listViewUsers = ui->listOfUsersStart;
    parseUsersToListView(listViewUsers, currentPath + "/users.txt");

    listModelQuizes = new QStringListModel(this);
    listViewQuizes = ui->listOfQuizes;
    loadFileToListView(listViewQuizes, currentPath + "/quizes.txt");

    listModelHighscore = new QStringListModel(this);
    listViewHighscore = ui->listOfUsersHghscores;
    loadFileToListView(listViewHighscore, currentPath + "/quizes.txt");

    connect(ui->startButton, &QPushButton::clicked, this, &Quiz::startButtonClicked);
    connect(ui->addUserButton, &QPushButton::clicked, this, &Quiz::addUserButtonClicked);
    connect(ui->listOfUsersHghscores, &QListView::clicked, this, &Quiz::handleItemClicked);

    startTab = ui->startTab;
    quizTab = ui->quizTab;
    highscoreTab = ui->highscoreTab;
    quizWidget = ui->quizWidget;
    quizWidget->setTabEnabled(0, true);
    quizWidget->setTabEnabled(1, false);
    quizWidget->setTabEnabled(2, true);
    textEditHighscore = ui->highScoreTextEdit;
    textEditHighscore->setReadOnly(true);
    quizWidget->setCurrentIndex(0);
}

Quiz::~Quiz()
{
    delete ui;
}
