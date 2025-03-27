// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UButton* StartGameButton;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	//class UButton* ReadyButton;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* LobbyCanvasPanel;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void SetUserTexture(class UTexture2D* Texture, int Index);

protected:
	virtual void NativeConstruct() override;



private:
	// Temp
	const FString LOBBY_LEVEL = "LobbyDevLevel";
	const FString PLAY_LEVEL = "Sushi1-3StageLevel";

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess="true"))
	void MoveToPlayLevel();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void Ready();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");

	template <typename T>
	T* FindChildWidget(const FString& name, UCanvasPanel* canvas);


};
