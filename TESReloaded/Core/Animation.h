#pragma once

class AnimSequenceSingleEx : public AnimSequenceSingle
{
public:
	AnimSequenceSingleEx();
	~AnimSequenceSingleEx();

	BSAnimGroupSequence*			ORAnim;	// 08

};

class AnimSequenceMultipleEx : public AnimSequenceMultiple
{
public:
	AnimSequenceMultipleEx();
	~AnimSequenceMultipleEx();

	BSAnimGroupSequence*			ORAnim;	// 08

};

class ActorAnimDataEx : public ActorAnimData
{
public:
	ActorAnimDataEx();
	~ActorAnimDataEx();
	
	NiTList<BSAnimGroupSequence>*	ORAnims;// DC

};

void CreateAnimationHook();