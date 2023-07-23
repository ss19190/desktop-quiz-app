/**
 * @file question.h
 * @author Sara Sobstyl
 * @brief header containg question class
 * @version 2.0
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <QString>
#include <QTextStream>
#include <array>
#include <QDebug>
#ifndef QUESTION_H
#define QUESTION_H

/**
 * @brief Parent class that represents a single question.
 * It contains a question, up to 4 possible answers to choose from,
 * and one correct answer.
 */
class Question
{
protected:
    /** Variable to store the question */
    QString question;
    /** Array to store the possible answers (maximum 4) */
    std::array<QString, 4> answers;
    /** Variable to store the correct answer */
    QString correct_answer;

public:
    /** Default constructor */
    Question() {}
    /** Getter method for retrieving the question */
    QString get_question() { return question; }
    /** Getter method for retrieving the possible answers */
    std::array<QString, 4> get_answers() { return answers; }
    /** Getter method for retrieving the correct answer */
    QString get_correct_answer() { return correct_answer; }
};

class Question4Ans : public Question
{
public:
    /** Default constructor */
    Question4Ans() {}
    /** Constructor to read question data from a QTextStream */
    Question4Ans(QTextStream &infile)
    {
        QString line = infile.readLine(); // Read the question
        question = line;
        for (size_t i = 0; i < 4; i++)
        {
            answers[i] = infile.readLine(); // Read each possible answer
        }
        correct_answer = infile.readLine(); // Read the correct answer
        infile.readLine();                  // Skip empty line

        // Debugging: Print the question, answers, and correct answer
        qDebug() << "Question: " << question << "\nAnswers: " << answers[0] << " " << answers[1] << " " << answers[2] << " " << answers[3] << " correct ans: " << correct_answer;
    }
};

class Question2Ans : public Question
{
public:
    /** Constructor to read question data from a QTextStream */
    Question2Ans(QTextStream &infile)
    {
        QString line = infile.readLine(); // Read the question
        question = line;
        correct_answer = infile.readLine(); // Read the correct answer
        infile.readLine();                  // Skip empty line

    }
};

#endif // QUESTION_H
