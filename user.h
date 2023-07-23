/**
 * @file user.h
 * @author Sara Sobstyl
 * @brief header containg user class
 * @version 2.0
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <QString>
#include <QTextStream>
#include <string>
#include <QDebug>

#ifndef USER_H
#define USER_H

/**
 * @brief The User class represents a user of the quiz application.
 * It stores the user's ID, name, and highscores for different quizzes.
 */
class User
{
private:
    /** User ID */
    int id;
    /** User name */
    std::string name;
    /** User's highscores for different quizzes */
    std::unordered_map<std::string, int> highscores;

public:
    User(){}
    User(QTextStream &inFile)
    {
        id = inFile.readLine().toInt();
        QString line = inFile.readLine();
        name = line.toStdString();
        line = inFile.readLine();
        while (!line.isEmpty())
        {
            std::string quiz = line.toStdString();
            line = inFile.readLine();
            qDebug() << line;
            int score = line.toInt();
            highscores[quiz] = score;
            line = inFile.readLine();
            qDebug() << line;
        }
        qDebug() << "name: " << QString::fromStdString(name);
    }




    /**
     * @brief Constructor for creating a User object with ID and name.
     * @param uid The ID of the user.
     * @param uname The name of the user.
     */
    User(int uid, std::string uname)
    {
        id = uid;
        name = uname;
        highscores = std::unordered_map<std::string, int>();
    }


    /**
     * @brief Sets the highscore for a specific quiz.
     * If the user already has a highscore for the quiz, it updates it only if the new score is higher.
     * @param quiz The name of the quiz.
     * @param score The score achieved in the quiz.
     */
    void set_highscore(std::string quiz, int score)
    {
        if (highscores.find(quiz) != highscores.end())
        {
            if (highscores[quiz] > score)
                highscores[quiz] = score;
            return;

        }
        highscores[quiz] = score;
    }

    /**
     * @brief Returns the highscores of the user for different quizzes.
     * @return A constant reference to the unordered map of highscores.
     */
    const std::unordered_map<std::string, int> get_highscores()
    {
        return highscores;
    }

    /**
     * @brief Returns the name of the user.
     * @return A constant reference to the user's name.
     */
    const std::string get_name() { return name; }

    /**
     * @brief Returns the ID of the user.
     * @return The user's ID.
     */
    const int get_id() { return id; }

    /**
     * @brief Saves the user's data to a QStringList.
     * @param lines The QStringList to store the user's data.
     * @return True if the saving process is successful, false otherwise.
     */
    bool save_to_string_list(QStringList &lines)
    {
        qDebug() << QString::number(id);
        lines.append(QString::number(id));
        qDebug() << QString::fromStdString(name);
        lines.append(QString::fromStdString(name));
        for (auto &score : highscores)
        {
            qDebug() << QString::fromStdString(score.first);
            lines.append(QString::fromStdString(score.first));
            qDebug() << QString::number(score.second);
            lines.append(QString::number(score.second));
        }
        lines.append("");
        qDebug()
            << "gites";
        return true;
    }

};


#endif
