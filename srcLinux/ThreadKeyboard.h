#ifndef THREADKEYBOARD_H
#define THREADKEYBOARD_H
#include <QThread>
#include <QTcpSocket>

/**
 * @brief The threadSend class
 * class to inform class when user write in the keyboard
 */
class ThreadKeyboard : public QThread {
    Q_OBJECT

public :
    /**
     * @brief threadSend
     * CONSTRUCTOR
     */
    ThreadKeyboard();

signals :
    /**
     * @brief emitMessage
     * emit this signal when user write in keyboard
     */
    void emitMessage();

    /**
     * @brief close
     * emit this signal when user write "!quit" in keyboard
     */
    void close();

private:
    /**
     * @brief run
     * method inherited of QThread, launching the thread
     */
    void run();
};

#endif // THREADKEYBOARD_H
