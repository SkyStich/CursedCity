// Fill out your copyright notice in the Description page of Project Settings.


#include "SingletonClass/GameSingletonClass.h"

UGameSingletonClass& UGameSingletonClass::Get()
{
	UGameSingletonClass* Singleton = Cast<UGameSingletonClass>(GEngine->GameSingleton);
    
	return Singleton ? *Singleton : *NewObject<UGameSingletonClass>(StaticClass());
}
