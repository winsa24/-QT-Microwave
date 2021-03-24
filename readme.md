# Qt Lab#3 : Statecharts

## Xiang WEI - IGD IP Paris


### Introduction
I implement the presentation and interaction control panel of a microwave oven on Qt.

### 1. Design the Statechart

### 2. Graphical interface
In this part, I created a graphical interface by dragging and dropping based on Qt Designor. I used QLineEdit and QDial as display panel and slider.

### 3. Implement
In this part, I first implement the QtStateMachine according to the Statechart shown in the first section. I created 9 states and use state->addTransition() to link different states with different buttons. Then I created each state a slot function and connect them together.

### 4. Clock
>In this part, I created two QTimer and used start or stop to control them depending on the state. Also I emited a customized signal when the cooking time runs out, and added transit from cook state to idle state when receiving this signal. 
```
start->addTransition(this,SIGNAL(cookfinished()),s11);
```

### 5. Improve GUI
>After completing all the steps above, I tried to show some heating effects by using animation. I implemented a color gradient effect by using QPropertyAnimation. It’s not straightforward because color is not a supported property so I have to add myself. Following are the code which I used to create a new property.
```
Q_PROPERTY(QColor color READ color WRITE setColor)

private:
    void setColor (QColor color);
    QColor color(){ //fake read function
      return Qt::black; 
    }
```

### Conclusion
By following all the steps, I had a better understanding of the state machine in Qt and also reviewed my knowledge on slots. I also learnt more about timer and animation in Qt and tried to find the link with doing them in web development. There is a screenshot of the final implementation.

## Final Result   
![img](https://github.com/winsa24/-QT-Microwave/blob/main/result.gif)   


> Useful Link for QAnimation.   
> https://www.cnblogs.com/cmt/p/14553189.html    
> https://blog.csdn.net/tiandyoin/article/details/105936390?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242    
> https://forum.qt.io/topic/23965/solved-how-can-i-know-what-animations-qpropertyanimation-can-do    
>  
