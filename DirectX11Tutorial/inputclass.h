#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
//#ifndef _INPUTCLASS_H_
//#define _INPUTCLASS_H_

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////

//Обрабатывает нажатие клавиш клавиатуры. Получает данные от MessageHandler. Объект ввода будет хранить состояние каждой клавиши и сообщать его вызывающим функциям
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

//#endif