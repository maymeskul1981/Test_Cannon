#include "stdafx.h"
#include <cmath>
#include "TargetsController.h"
#include "TestWidget.h"

// ------------- Target --------------------
Target::Target(FPoint position, float speed) {
	// Установка свойств
	_position = position;
	// Скорость задаем случайным образом
	
	_speed = math::random(10, 5 * int(speed));
	// Случайным образом задаем направление начальной скорости
	_speedVector.x = math::random(-1.0f, 1.0f);
	_speedVector.y = sqrt(1 - _speedVector.x * _speedVector.x);
	//_speedVector.y = math::random(-1.0f, 1.0f);
	_mass = 1.0f;
	_speedVector.x *= _speed;
	_speedVector.y *= _speed;
	// Задаем случайный масштаб текстуры
	//_scale = 0.1f;
	_scale = math::random(0.1f, 0.3f);
	_hitting = false;
	_life = 4;
	
	// Случайным образом выбираем текстуру из 4ех предложенных
	int bat;
	bat = math::random(0, 4);
	switch (bat) {
	case 0:
		_trg = Core::resourceManager.Get<Render::Texture>("target1");
		break;
	case 1:
		_trg = Core::resourceManager.Get<Render::Texture>("target2");
		break;
	case 2:
		_trg = Core::resourceManager.Get<Render::Texture>("target3");
		break;
	case 3:
		_trg = Core::resourceManager.Get<Render::Texture>("target4");
		break;
	case 4:
		_trg = Core::resourceManager.Get<Render::Texture>("target5");
		break;
	default:
		_trg = Core::resourceManager.Get<Render::Texture>("target1");
		break;
	}
}

void Target::Draw() {
	// Если попадания не было
	if (_life > 0) {
		// Рисуем мишень
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);
		IRect texRect = _trg->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_trg->TranslateUV(rect, uv);
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));
		_trg->Bind();
		Render::DrawQuad(rect, uv);
		Render::device.PopMatrix();
	}
}

void Target::Update(float dt) {
	IRect texRect = _trg->getBitmapRect();
	// Высчитываем положение мишении относительно краев экрана и меняем его, 
	// чтобы получить эффект отскакивания
	if ((_position.x + _speedVector.x * dt) > (TestWidget::WINDOW_WIDTH - texRect.width/2 * _scale) ||
		((_position.x + _speedVector.x * dt) < (texRect.width/2 * _scale))) {
		_speedVector.x = -_speedVector.x;
	}
	if ((_position.y + _speedVector.y * dt) > (TestWidget::WINDOW_HEIGHT - texRect.height/2 * _scale) ||
		((_position.y + _speedVector.y * dt) < (texRect.height/2 * _scale))) {
		_speedVector.y = -_speedVector.y;
	}

	_position.x += _speedVector.x * dt;
	_position.y += _speedVector.y * dt;

	if (_life == 0) {
		_scale = 0;		
	}	
}

FPoint Target::getPosition() {
	return _position;
}

IRect Target::getRect()
{
	IRect texRect = _trg->getBitmapRect();
	return texRect;
}

FPoint Target::getSpeedVector() {
	return _speedVector;
}

float Target::getScaleTarget()
{
	return _scale;
}

float Target::getMassTarget()
{
	return _mass;
}

bool Target::hitOver()
{
	_life = _life - 1;
	return (_life <= 0);
}

void Target::setSpeedVector(FPoint speedVector) {
	_speedVector = speedVector;
}

void Target::setPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;
}


// Проверка попадания в мишень
bool Target::chekHit(FPoint posBullet, EffectsContainer &effCont) {
	bool status = false;
	IRect texRect = _trg->getBitmapRect();
	// Если точка входить в RECT текстуры,
	if (((posBullet.x) >= (_position.x - texRect.width / 2 * _scale)) &&
		((posBullet.y) >= (_position.y - texRect.height / 2 * _scale)) &&
		((posBullet.x) <= (_position.x - texRect.width / 2 * _scale + texRect.width / 2 * _scale * 2)) &&
		((posBullet.y) <= (_position.y - texRect.height / 2 * _scale + texRect.height / 2 * _scale * 2))) {
		// То будем считать, что попали
		//_hitting = true;
		status = true;
		if (_life == 1)
		{			
			_hitting = true;
			// И запускаем эффект "сгорания мишени"
			ParticleEffectPtr eff = effCont.AddEffect("Puh");
			eff->posX = _position.x + 0.f;
			eff->posY = _position.y + 0.f;
			eff->Reset();
		}
	}
	return status;
}

bool Target::getHit() {
	return _hitting;
}

Target::~Target() {
	_effCont.Finish();
	_effCont.KillAllEffects();
	//_effCont = NULL;
}


// ---------------------------  TargetsController Class --------------------
// Конструктор с параметрами

TargetsController::TargetsController(int count, float timeLimit, float speed)
{
	_count = count;
	_timeLimit = timeLimit;
	_speed = speed;
	Init();
}

void TargetsController::Init() {
	Core::Application *app = Core::appInstance;
	for (int i = 0; i < _count; i++) {
		
		// Создаем необходимое количество мишеней		
		_targets.push_back(CheckNewTarget());		
	}
}

void TargetsController::Draw() {
	// Рисуем мишени
	for (unsigned i = 0; i < _targets.size(); i++) {
		_targets[i]->Draw();
	}
}

Target* TargetsController::CheckNewTarget() {
		
	float distance, x, y;
	Target* trg = new Target(FPoint(math::random(256, 600), math::random(256, 600)), _speed);
	x = math::random(trg->getRect().width / 2 * trg->getScaleTarget(), TestWidget::WINDOW_WIDTH - trg->getRect().width / 2 * trg->getScaleTarget());
	y = math::random(trg->getRect().width / 2 * trg->getScaleTarget(), TestWidget::WINDOW_HEIGHT - trg->getRect().width / 2 * trg->getScaleTarget());
	trg->setPosition(x, y);
	
	
		bool foundPlace = false;
		bool collision = false;		
		while (!foundPlace)
		{
			if (collision)
			{
				x = math::random(trg->getRect().width / 2 * trg->getScaleTarget(), TestWidget::WINDOW_WIDTH - trg->getRect().width / 2 * trg->getScaleTarget());
				y = math::random(trg->getRect().width / 2 * trg->getScaleTarget(), TestWidget::WINDOW_HEIGHT - trg->getRect().width / 2 * trg->getScaleTarget());
				trg->setPosition(x, y);
				collision = false;
			}
			
			for (unsigned i = 0; i < _targets.size(); i++)
			{
				distance = _targets[i]->getPosition().GetDistanceTo(trg->getPosition());
				float nPos = trg->getRect().width / 2 * trg->getScaleTarget() + _targets[i]->getRect().width / 2 * _targets[i]->getScaleTarget();
				if (distance <= (nPos))
				{
					collision = true;
					break;
				}				
			}
			if (!collision)
			{
				foundPlace = true;
			}
		}
		return trg;
}

// Получить количество мишений в данный момент
int TargetsController::GetCountTargetNow() {
	int countNow = 0;

	if (_targets.size() == 0) {
		return 0;
	}

	for (int i = _targets.size() - 1; i >= 0; i--) {
		// Если в мишень еще не попали	
		if (!_targets[i]->getHit()) {
			// то считаем ее
			countNow++;
		}
	}
	return countNow;
}

void TargetsController::Update(float dt) {	
	for (unsigned i = 0; i < _targets.size(); i++) {
		_targets[i]->Update(dt);
		IRect texRectCurrent = _targets[i]->getRect();
		FPoint posCurrent = _targets[i]->getPosition();
		FPoint sVectorCurrent = _targets[i]->getSpeedVector();
		float scaleCurrent = _targets[i]->getScaleTarget();
		for (unsigned j = 0; j < _targets.size(); j++) {
			if (i != j)
			{
				IRect texRectOther = _targets[j]->getRect();
				FPoint posOther = _targets[j]->getPosition();
				FPoint sVectorOther = _targets[j]->getSpeedVector();
				float scaleOther = _targets[j]->getScaleTarget();				
				bool collision = false;
				float distance = posOther.GetDistanceTo(posCurrent);
				if (distance <= (texRectCurrent.width / 2 * scaleCurrent + texRectOther.width / 2 * scaleOther))
				{
					collision = true;
				}
				if (collision)
				{
					float ma = _targets[i]->getMassTarget();
					float mb = _targets[j]->getMassTarget();
					//FPoint unitNormalVector = distance.Normalize();
					float nabx = posOther.x - posCurrent.x;
					float naby = posOther.y - posCurrent.y;
					nabx /= distance;
					naby /= distance;
					float tabx = -naby;
					float taby = nabx;
					float vain = sVectorCurrent.x * nabx + sVectorCurrent.y * naby;
					float vait = sVectorCurrent.x * tabx + sVectorCurrent.y * taby;
					float vbin = sVectorOther.x * nabx + sVectorOther.y * naby;
					float vbit = sVectorOther.x * tabx + sVectorOther.y * taby;
					float cof_E = 1;
					float vafn = (mb*vbin*(cof_E + 1) + vain * (ma - cof_E * mb)) / (ma + mb);
					float vbfn = (ma*vain*(cof_E + 1) - vbin * (mb - cof_E * mb)) / (ma + mb);
					float vaft = vait;
					float vbft = vbit;
					float xfa = vafn * nabx + vaft * tabx;
					float yfa = vafn * naby + vaft * taby;
					float xfb = vbfn * nabx + vbft * tabx;
					float yfb = vbfn * naby + vbft * taby;
					sVectorCurrent.x = xfa;
					sVectorCurrent.y = yfa;
					sVectorOther.x = xfb;
					sVectorOther.y = yfb;
					_targets[i]->setSpeedVector(sVectorCurrent);
					_targets[j]->setSpeedVector(sVectorOther);
					_targets[i]->setPosition(posCurrent.x + xfa*dt, posCurrent.y + yfa*dt);
					_targets[j]->setPosition(posOther.x + xfb*dt, posOther.y + yfb*dt);
				}
			}
		}		
	}
}

void TargetsController::Clear() {
	for (int i = _targets.size() - 1; i >= 0; i--) {
		delete _targets[i];
	}
	_targets.clear();
}

bool TargetsController::ChekingHits(FPoint posArrow, EffectsContainer &effCont) {
	// Временная переменная
	bool res = false;
	for (int i = _targets.size() - 1; i >= 0; i--) {
		// Если в мишень еще не попали
		//if (!_targets[i]->getHit()) {
		//if (!_targets[i]->getHit()) {
			// Проверяем попадание этой точки
		if (_targets[i]->chekHit(posArrow, effCont)) {
			res = true;
			// Удаляем мишень из вектора
			if (_targets[i]->hitOver()) {
				delete _targets[i];
				_targets.erase(_targets.begin() + i);
				
			}
				// Выходим из цикла
				//return res;
			break;
		}
		//}
	}
	return res;
}