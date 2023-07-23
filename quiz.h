/**
 * @file quiz.h
 * @author Sara Sobstyl
 * @brief header containg quiz class
 * @version 2.0
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "question.h"
#include "user.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QListView>
#include <QStringListModel>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <string>
#include <unordered_map>
#include <limits>
#include <random>
#include <chrono>
#include <QtWidgets>
#include <QDir>

#ifndef QUIZ_H
#define QUIZ_H

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Quiz;
}
QT_END_NAMESPACE

/**
 * @brief The Quiz class represents the main window of the quiz application.
 * It handles user interactions and manages the quiz functionality.
 */
class Quiz : public QMainWindow
{
    Q_OBJECT
    /** Hash map to store user data */
    std::unordered_map<int, User> users;

public:
    Quiz(QWidget *parent = nullptr);
    ~Quiz();

private slots:
    void startButtonClicked();
    void addUserButtonClicked();
    void seeHighscoreClicked();
    void handleItemClicked(const QModelIndex &index);

private:
    Ui::Quiz *ui;
    QListView *listViewUsers;
    QListView *listViewQuizes;
    QListView *listViewHighscore;
    QStringListModel *listModelUsers;
    QStringListModel *listModelQuizes;
    QStringListModel *listModelHighscore;
    QTextEdit *textEditUser;
    QTextEdit *textEditHighscore;
    QLabel *questionaLabel;
    QLabel *username;
    QLabel *questionCounter;
    QLabel *scoreLabel;
    QLabel *highscoreLabel;
    QPushButton *buttonA;
    QPushButton *buttonB;
    QPushButton *buttonC;
    QPushButton *buttonD;
    QPushButton *startButton;
    QWidget *startTab;
    QWidget *quizTab;
    QWidget *highscoreTab;
    QTabWidget *quizWidget;
    QRadioButton *ABCDButton;
    QRadioButton *buttonTrue;

    void loadFileToListView(QListView *listView, QString filePath);
    std::unordered_map<int, User> createUsers();
    void showHighscore(std::unordered_map<int, User> users, QString quiz);
    std::unordered_map<int, Question> createQuiz(QString filePath);
    std::pair<QString, QString> getInformation();
    int get_random_in_range(int from = std::numeric_limits<int>::min(), int to = std::numeric_limits<int>::max());
    void changeUserScore(const QString &userName, const QString &quizName, int newScore);
    void parseUsersToListView(QListView *listView, QString filePath);
    void runQuizABCD();
    void runQuizTrueOrFalse();
};

#endif // QUIZ_H
