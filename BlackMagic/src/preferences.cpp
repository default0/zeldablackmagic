#include "preferences.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpacerItem>
#include <QPushButton>

Preferences::Preferences(QWidget* parent)
{
    setWindowTitle(tr("Preferences"));
    hide();
    setupGui();
    show();
}

void Preferences::setupGui()
{
    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout;
    QVBoxLayout* mainVBox = new QVBoxLayout;
    m_groupMapEdit = new QGroupBox;
    m_groupMapEdit->setTitle("Map Editor");
    m_groupMapEdit->setLayout(vbox);
    m_cbAnimation = new QCheckBox(tr("Enable Animation"));
    m_cbGrid = new QCheckBox(tr("Display Grid"));
    vbox->addWidget(m_cbAnimation);
    vbox->addWidget(m_cbGrid);

    m_groupTextEdit = new QGroupBox;
    QVBoxLayout* vbox1 = new QVBoxLayout;
    m_groupTextEdit->setTitle("Text Editor");
    m_cbLineNumbers = new QCheckBox("Show Line Numbers");
    vbox1->addWidget(m_cbLineNumbers);
    vbox1->addStretch(0);
    m_groupTextEdit->setLayout(vbox1);
    hbox->addWidget(m_groupMapEdit);
    hbox->addWidget(m_groupTextEdit);

    mainVBox->addLayout(hbox);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(0);
    QPushButton* m_bOk = new QPushButton("&OK");
    QPushButton* m_bCancel = new QPushButton("&Cancel");
    connect(m_bOk, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_bCancel, SIGNAL(clicked()), this, SLOT(close()));
    buttonLayout->addWidget(m_bOk);
    buttonLayout->addWidget(m_bCancel);
    mainVBox->addLayout(buttonLayout);

    setLayout(mainVBox);
}
