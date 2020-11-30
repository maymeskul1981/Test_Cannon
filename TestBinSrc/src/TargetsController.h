#pragma once

#include <vector>

//------------------------ Target Class --------------------------
// Класс мишени
class Target {
public:
	// Конструктор с параметром начальной позиции
	Target(FPoint position, float speed);
	~Target();
	void Draw();
	void Update(float dt);

	// Получить позицию мишени
	FPoint getPosition();
	FPoint getSpeedVector();
	IRect getRect();
	void setSpeedVector(FPoint speedVector);
	void setPosition(float x, float y);
	float getScaleTarget();
	float getMassTarget();	
	bool hitOver();

	// Проверить попадание в мишень
	bool chekHit(FPoint posArrow, EffectsContainer &effCont);
	// Получить значение попадания
	bool getHit();
	
private:
	// Текстура мишени
	Render::Texture *_trg;
	// Позиция мишени
	FPoint _position;
	// Скорость 
	float _speed;
	//Масса
	float _mass;
	// Масштаб текстуры
	float _scale;
	// Вектор скорости
	FPoint _speedVector;
	// Флаг "попадание"
	bool _hitting;
	int _life;
	
	// Контейнер с эффектами
	EffectsContainer	_effCont;
	
};

//-------------------------- Targets Class ----------------
// Класс управления мишенями

class TargetsController
{
public:
	// Конструктор с параметрами количества мишеней и ограничение по времени
	TargetsController(int count, float timeLimit, float speed);	// Инициализация	 
	void Init();
	void Draw();
	void Update(float dt);
	// Проверка попадания точки в мишени
	bool ChekingHits(FPoint posBullet, EffectsContainer &effCont);

	// Очистка вектора мишеней
	void Clear();
	// Получить количество мишеней в текущий момент
	int GetCountTargetNow();	
	Target* TargetsController::CheckNewTarget();

private:
	// Вектор с мишенями
	std::vector <Target *> _targets;
	// Количество
	int _count;
	float _speed;
	// Ограничение по времени
	float _timeLimit;
};