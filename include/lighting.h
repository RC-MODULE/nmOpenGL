#pragma once
#include "nmpp.h"

extern "C" void mulC_v4nm32f(v4nm32f * pSrcV, v4nm32f * pSrcC, v4nm32f * pDst, int size);

struct LightingInfo {
	v4nm32f ambientMul[MAX_LIGHTS + 1];    ///< Общие значения окружающей интенсивности материала и источников освещения (элемент MAX_LIGHTS говорит об общей интенсивности материала и сцены)
	v4nm32f diffuseMul[MAX_LIGHTS];		   ///< Общие значения рассеяной интенсивности материала и источников освещения
	v4nm32f specularMul[MAX_LIGHTS];	   ///< Общие значения зеркальной интенсивности материала и источников освещения

	v4nm32f materialAmbient;			///< Окружающий цвет материала
	v4nm32f materialDiffuse;			///< Рассеяный цвет материала
	v4nm32f materialSpecular;			///< Зеркальный цвет материала
	v4nm32f materialEmissive;			///< Эмиссионный цвет материала
	v4nm32f *pMaterialAmbient;	///< Указатель на окружающий цвет материала (materialAmbient), используется в GetMaterialfv и в формуле расчёта цветапри включенном освещении
	v4nm32f *pMaterialDiffuse;	///< Указатель на зеркальный цвет материала (materialDiffuse), используется в GetMaterialfv и в формуле расчёта цветапри включенном освещении

	v4nm32f lightAmbient[MAX_LIGHTS + 1];   ///< Значения окружающей интенсивности источников света (элемент MAX_LIGHTS говорит об интенсивности сцены)
	v4nm32f lightDiffuse[MAX_LIGHTS];       ///< Значения рассеяной интенсивности источников света
	v4nm32f lightSpecular[MAX_LIGHTS];      ///< Значения зеркальной интенсивности источников света
	v4nm32f lightPosition[MAX_LIGHTS];		///< Значения положения источников освещения
	v4nm32f lightSpotDirection[MAX_LIGHTS];		///< Направление прожектора для источников света
	float lightSpotExp[MAX_LIGHTS];			///< Показатель степени прожектора для источников света
	float lightSpotCutoff[MAX_LIGHTS];		///< Угол отсечки прожектора для источников света
	float lightConstAtt[MAX_LIGHTS];		///< Постоянный коэффициент затухания для источника света (не используется)
	float lightLinAtt[MAX_LIGHTS];			///< Коэффициент линейного затухания  для источника света (не используется)
	float lightQuadAtt[MAX_LIGHTS];			///< Квадратичный коэффициент затухания для источника света (не используется)
	int isEnabledLight[MAX_LIGHTS];		///< Флаги активности источников света
	int isLighting;						///< Флаг активности расчета освещения
	int isColorMaterial;				///< Флаг активности режима COLOR_MATERIAL
	float specularExp;					///< Показатель зеркальности
	int dummy;

	void update(){
		mulC_v4nm32f(lightSpecular, &materialSpecular, specularMul, MAX_LIGHTS);
		if (isColorMaterial == 0) {
			mulC_v4nm32f(lightAmbient, &materialAmbient, ambientMul, MAX_LIGHTS + 1);
			mulC_v4nm32f(lightDiffuse, &materialDiffuse, diffuseMul, MAX_LIGHTS);
			nmppsAdd_32f((float*)(ambientMul + MAX_LIGHTS), 
						(float*)&materialEmissive, 
						(float*)(ambientMul + MAX_LIGHTS), 4);
		}
	}

	void init() {
		specularExp = 0;
		isLighting = NMGL_FALSE;
		isColorMaterial = NMGL_FALSE;

		materialAmbient.vec[0] = 0.2f;
		materialAmbient.vec[1] = 0.2f;
		materialAmbient.vec[2] = 0.2f;
		materialAmbient.vec[3] = 1.0f;
		materialDiffuse.vec[0] = 0.8f;
		materialDiffuse.vec[1] = 0.8f;
		materialDiffuse.vec[2] = 0.8f;
		materialDiffuse.vec[3] = 1.0f;
		materialSpecular.vec[0] = 0.0f;
		materialSpecular.vec[1] = 0.0f;
		materialSpecular.vec[2] = 0.0f;
		materialSpecular.vec[3] = 1.0f;
		materialEmissive.vec[0] = 0.0f;
		materialEmissive.vec[1] = 0.0f;
		materialEmissive.vec[2] = 0.0f;
		materialEmissive.vec[3] = 1.0f;
		pMaterialAmbient = &materialAmbient;
		pMaterialDiffuse = &materialDiffuse;

		for (int i = 0; i < MAX_LIGHTS; i++) {
			if (i == 0) {
				lightDiffuse[i].vec[0] = 1.0f;
				lightDiffuse[i].vec[1] = 1.0f;
				lightDiffuse[i].vec[2] = 1.0f;
				lightDiffuse[i].vec[3] = 1.0f;

				lightSpecular[i].vec[0] = 1.0f;
				lightSpecular[i].vec[1] = 1.0f;
				lightSpecular[i].vec[2] = 1.0f;
				lightSpecular[i].vec[3] = 1.0f;
			}
			else {
				lightDiffuse[i].vec[0] = 0.0f;
				lightDiffuse[i].vec[1] = 0.0f;
				lightDiffuse[i].vec[2] = 0.0f;
				lightDiffuse[i].vec[3] = 1.0f;

				lightSpecular[i].vec[0] = 0.0f;
				lightSpecular[i].vec[1] = 0.0f;
				lightSpecular[i].vec[2] = 0.0f;
				lightSpecular[i].vec[3] = 1.0f;
			}

			lightAmbient[i].vec[0] = 0.0f;
			lightAmbient[i].vec[1] = 0.0f;
			lightAmbient[i].vec[2] = 0.0f;
			lightAmbient[i].vec[3] = 1.0f;

			lightPosition[i].vec[0] = 0.0f;
			lightPosition[i].vec[1] = 0.0f;
			lightPosition[i].vec[2] = 1.0f;
			lightPosition[i].vec[3] = 0.0f;

			lightSpotDirection[i].vec[0] = 0.0f;
			lightSpotDirection[i].vec[1] = 0.0f;
			lightSpotDirection[i].vec[2] = -1.0f;
			lightSpotDirection[i].vec[3] = 0.0f;

			lightSpotExp[i] = 0.0f;
			lightSpotCutoff[i] = 180.0f;
			lightConstAtt[i] = 1.0f;
			lightLinAtt[i] = 0.0f;
			lightQuadAtt[i] = 0.0f;
			isEnabledLight[i] = false;
		}

		lightAmbient[MAX_LIGHTS].vec[0] = 0.2f;
		lightAmbient[MAX_LIGHTS].vec[1] = 0.2f;
		lightAmbient[MAX_LIGHTS].vec[2] = 0.2f;
		lightAmbient[MAX_LIGHTS].vec[3] = 1.0f;
	}
};
