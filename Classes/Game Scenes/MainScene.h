//
//  MainScene.h
//  ComboBlocks
//
//  Created by 有働 篤人 on 11/28/14.
//
//

#ifndef ComboBlocks_MainScene_h
#define ComboBlocks_MainScene_h

#include "cocos2d.h"
#include "ADX2Manager.h"
#include "Blocks.h"
#include "ScoreItemLabel.h"

typedef cocos2d::Vector<Blocks *> BlockVct;

class MainScene :public cocos2d::Layer
{
protected:
    MainScene(); //constructor
    virtual ~MainScene(); //destructor
    bool init() override; //initialization
    
    
    
public:
    
    /** Game State
     *  INPLAY : Now playing
     *  RESULT : Result Screen
     */
    enum class GameState{
        PLAYING,
        RESULT,
        HAZARDFAIL
    };
    
    /** Game Mode
     *  NORMAL : Basic Rules applied.
     *  HAZARD : Get the combo of specific length. Fail, and game over.
     */
    enum class GameMode{
        NORMAL,
        HAZARD
    };
    
    void update(float dt); //update method
    void onEnterTransitionDidFinish() override; //called when transisiton is over
    
    /** Add block specified
     *  @param block to be added
     */
    void addBlock(Blocks *block);
    
    /** tells the sort of the block at the designated position IN GRID
     *   nullptr if nothing found
     *  @param position position of blocks IN GRID
     *  @return Sort of block OR nullptr
     */
    Blocks* getBlockAt(const cocos2d::Vec2& pos);
    
    /** tells the sort of the block at the designated position IN PIXEL
     *  nullptr if nothing found
     *  @param position position of blocks IN PIXEL
     *  @return Sort of block OR nullptr
     */
    Blocks* getBlockAtByPixel(const cocos2d::Vec2& pixelPos);
    
    /** Move the block into designated position IN GRID
     *  @param block0 Block to be moved
     *  @param destination The name says it all
     */
    void moveBlock(Blocks* block0, const cocos2d::Vec2& destination);
    
    /** Swap 2 blocks specified
     *  @param block0 The block specified
     *  @param block1
     */
    bool swapBlocks(Blocks* block0, Blocks* block1);
    
    
    /** Check if a specified block can be deleted. This will only check "3-block deletion," and this actually works well
     *  @param block Starting block
     *  @return Vector containing the block to be deleted
     */
    BlockVct checkNeighboringBlocks(Blocks* block);
    
    /** Checks if any block on the field can be deleted, and if so, set them as DELETED
     *  @return whether they are deleted or not
     */
    bool checkDeletion();
    
    /** Checks if any block on the field can be deleted, but this is for anti-instant delete.
     *  @param block the block in question
     *  @return whether the block in question can be deleted or not.
     *  @remarks if this returns true, then that block must be changed into a different block
     */
    bool checkDeletion(Blocks* block);
    
    /** remove given blocks form the field
     *  effect appiled.
     *  @param block to be deleted
     */
    void deleteBlock(Blocks* block);
    
    
    /** checks if the block can be deleted, and if it can, drops it.
     *  @param block in question
     *  @return whether it fell or not
     */
    bool dropBlock(Blocks *block);
    
    /** look for the empty spot in the field, and returns the block spawned.
     *  if nothing spawned, an empty vector will be returned
     *  @return list of block spawned
     */
    cocos2d::Vector<Blocks*> checkSpawn();
    
    /** determines if music should change - by means of ComboLevel.
     * @return TRUE if it's time to change
     */
    bool shouldChangeMusic();
    
    bool removeScoreItemLabel(ScoreItemLabel * label);
    
    
    /** What should be done just after the time runs out.
     */
    void onResult();
    
    /** Passes on to next scene.
     */
    void passOn();
    
    //things to do when creating this scene
    static cocos2d::Scene * createScene();
    CREATE_FUNC(MainScene);
    
    
    /// Current Score
    CC_SYNTHESIZE(int, _score, Score);
    // Score for animation
    CC_SYNTHESIZE(int, _aniScore, AniScore);
    /// Current Combo
    CC_SYNTHESIZE(int, _combo, Combo);
    /// Big Explosion Counter (Simultaneous deletion)
    CC_SYNTHESIZE(int, _simCount, SimCount);
    /// Time
    CC_SYNTHESIZE(float, _time, Time);
    /// Combo Timeout Timer (based on frames)
    CC_SYNTHESIZE(int, _comboTimeout, ComboTimeout);
    /// Combo Timeout Timer for actual display
    CC_SYNTHESIZE(int, _aniComboTimeout, AniComboTimeout);
    /// Combo Level (determines the BGM)
    CC_SYNTHESIZE(float, _comboLevel, ComboLevel);
    /// Current Combo Level (Load Reduction)
    CC_SYNTHESIZE(int, _curComboLevel, CurComboLevel);
    /// Did the player start destroying blocks?
    CC_SYNTHESIZE(bool, _hasStarted, HasStarted);
    //Score Label (Atlaslabel) (Pointer)
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
    //Time Label (AtlasLabel) (Pointer)
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _timeLabel, TimeLabel);
    //Main Combo Label (AtlasLabel) (Pointer)
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _comboLabel, ComboLabel);
    //Fail Combo Label (AtlasLabel) (This will be blown away when combo is broken)
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _failComboLabel, FailComboLabel);
    //Score Item Labels (AtlasLabel) (These will fly towards the score when player deletes the blocks)
    CC_SYNTHESIZE(cocos2d::Vector<ScoreItemLabel *>, _scoreItemLabels, ScoreItemLabels);
    //Combo Timeout Bar (Temp)
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite * , _ctoBar, CTOBar);
    /// Vector for saving blocks
    CC_SYNTHESIZE(BlockVct, _blocks, Blocks);
    /// Node to put blocks (Pointer)
    CC_SYNTHESIZE_RETAIN(cocos2d::Node*, _playField, PlayField);
    /// Block being selected (Pointer)
    CC_SYNTHESIZE_RETAIN(Blocks *, _curBlock, CurBlock);
    /// Current State of Gameplay
    CC_SYNTHESIZE(GameState, _state, State);
    /// Current Game Mode
    CC_SYNTHESIZE(GameMode, _gameMode, GameMode);
    /// ADX2 Cue Sheet (Pointer)
    CC_SYNTHESIZE_RETAIN(ADX2::CueSheet *, _cueSheet, CueSheet);
    
    //Main BGM Play ID
    CC_SYNTHESIZE(CriAtomExPlaybackId, _bgmPBID, BgmPBID);
    
};

#endif
