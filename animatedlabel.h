#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include <QLabel>

class AnimatedLabel : public QLabel
{

  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)

public:
  AnimatedLabel(QWidget *parent = 0)
  {
  }
  void setColor (QColor color){
    setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
  }
  QColor color(){
    return Qt::black; // getter is not really needed for now
  }
};


#endif // ANIMATEDLABEL_H
