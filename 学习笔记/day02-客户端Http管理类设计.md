## 完善注册类界面
先在注册类构造函数里添加lineEdit的模式为密码模式
``` cpp
ui->lineEdit_Passwd->setEchoMode(QLineEdit::Password);
ui->lineEdit_Confirm->setEchoMode(QLineEdit::Password);
```
我们在注册界面的ui里添加一个widget，widget内部包含一个tip居中显示，用来提示错误。