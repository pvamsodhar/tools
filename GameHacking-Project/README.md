<b>My First attempt:</b>
I thought to change attack damage of bears and angrybears.
<code>
virtual int32_t GetAttackDamage() override;
</code>
This function seems to be responsible for calculating attack damage.
<code>
int32_t Bear::GetAttackDamage(){
  return 0;
}
</code>
Still, Bears can attack me and decrease my health.

<b>My Second attempt:</b>

I thought to fly but stay in bound. So i have click on jump all the time and when player is falling i have to click jump again. But after sometime bears got armed.

<b>My Third attempt:</b>
There is function called CanBeArmed in Bear and AngryBear class so i thought to make it return zero.
But still they are getting armed after some time. I guess CanBeArmed function returns one due to someother function in libGameLogic.so after few minutes.

<b>My Fourth attempt:</b>
This time i thought to use the teleport option to teleport into places where it is physically not possible for bears to attack me.
I teleported into ground after starting the quest but fell out of bounds. So i thought to keep on jumping to stay in bound but in some jumps player accidentally comes out and gets launched out very far.
After that i noticed a tree near chest so I teleported into the tree after starting the quest and it worked. I had to wait there for five minutes, after completion i just jumped so high, bears despawned and i opened the chest. With AWM that I got from chest, I killed 10-20 bears cuz I have unbearable anger on those bears.

<b>My Fifth attempt:</b>
Some Function in Player Class(My friend noticed these functions).
<code>
virtual IQuest ** GetQuestList(size_t *);
virtual void FreeQuestList(IQuest **);
virtual IQuest * GetCurrentQuest(void);
virtual PlayerQuestState GetStateForQuest(IQuest *);
virtual bool IsQuestStarted(IQuest *) override;
virtual bool IsQuestCompleted(IQuest *);
virtual void SetCurrentQuest(IQuest *);
virtual void StartQuest(IQuest *) override;
virtual void AdvanceQuestToState(IQuest *, IQuestState *);
virtual void CompleteQuest(IQuest *) override;
</code>
I thought to complete the quest using CompleteQuest function when the quest starts.
So i wrote the code below in World::Tick function.
<code>
if (player != nullptr) {
  IQuest* currentQuest = player->GetCurrentQuest();
  if (currentQuest != nullptr){
    if (player->IsQuestStarted(currentQuest)) {
      player->CompleteQuest(currentQuest);
    }
  }
}
</code>
But the game is crashing when i click on join button and in crash report i saw this error 'Exception was "SIGSEGV: invalid attempt to access memory at address 0x00000268"'.
