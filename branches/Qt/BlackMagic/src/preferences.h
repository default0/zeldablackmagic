#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>

class QGroupBox;
class QCheckBox;
class QPushButton;

class Preferences : public QWidget
{
    //Q_OBJECT
public:
    Preferences(QWidget* parent);

public:
    void update();

private:
    void setupGui();

    QGroupBox* m_groupMapEdit;
    QGroupBox* m_groupTextEdit;

    // Map Editor options
    QCheckBox* m_cbAnimation;
    QCheckBox* m_cbGrid;

    // Text (or script) Editor options
    QCheckBox* m_cbLineNumbers;

    //QPushButton* m_bOk;
    //QPushButton* m_bCancel;
};

#endif // PREFERENCES_H
