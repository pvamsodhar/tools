#ifndef LIBGAMELOGIC_H
#define LIBGAMELOGIC_H


#include <map>
#include <set>
#include <cstdint>
#include <memory>
#include <cmath>

class Player;
class World;
class ClientWorld;
class ILocalPlayer;
class Actor;
class IItem;
class IInventory;
class IQuest;
class IQuestState;
class IPlayer;
class WriteStream;
class AIZone;
class Spawner;
class IUE4Actor;
class IActor;
class IAchievement;
class IFastTravel;
class WriteStream;
class NPC;
class AIState;
class Bear;
class BearChest;
class AIActor;

struct Vector3;
struct Rotation;
struct ItemAndCount;
struct QuestStateInfo;
struct ItemCountInfo;
struct PlayerQuestState;
struct LocationAndRotation;

enum DamageType {PhysicalDamage, FireDamage, ColdDamage, ShockDamage};
enum EnemyRank {NormalEnemy, EliteEnemy, LegendaryEnemy};


struct Vector3 {
    float x;
    float y;
    float z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    float MagnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    float Magnitude() const {
        return sqrt(MagnitudeSquared());
    }

    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (a - b).MagnitudeSquared();
    }

    static float Distance(const Vector3& a, const Vector3& b) {
        return (a - b).Magnitude();
    }

    void Normalize() {
        float mag = Magnitude();
        if (mag > 0) {
            *this *= (1.0f / mag);
        }
    }

    static Vector3 Normalize(const Vector3& vec) {
        Vector3 v = vec;
        v.Normalize();
        return v;
    }
};

struct Rotation {
    float pitch;
    float yaw;
    float roll;

    Rotation() : pitch(0), yaw(0), roll(0) {}
    Rotation(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}
};


template <typename T>
class ActorRef {
public:
    T* m_object;

public:
    ActorRef() : m_object(nullptr) {}
    ActorRef(T* object) : m_object(object) {}
    ActorRef(const ActorRef<T>& other) : m_object(other.m_object) {}

    ActorRef<T>& operator=(T* object) {
        m_object = object;
        return *this;
    }

    ActorRef<T>& operator=(const ActorRef<T>& other) {
        if (this != &other) {
            m_object = other.m_object;
        }
        return *this;
    }

    T* operator->() const {
        return m_object;
    }

    operator bool() const {
        return m_object != nullptr;
    }

    operator T*() const {
        return m_object;
    }

    T* Get() const {
        return m_object;
    }

    bool operator<(const ActorRef<T>& other) const {
        return m_object < other.m_object;
    }
};

class World {
public:
    std::set<ActorRef<IPlayer>> m_players;
    std::set<ActorRef<IActor>> m_actors;
    std::map<unsigned int, ActorRef<IActor>> m_actorsById;
    ILocalPlayer* m_localPlayer;
    uint32_t m_nextId;
    std::map<std::string, AIZone*> m_aiZones;

    void AddActorToWorld(Actor*);
    void AddActorToWorldWithId(uint32_t, Actor*);
    void SendEventToAllPlayers(const WriteStream&);
    void SendEventToAllPlayersExcept(Player*, const WriteStream&);

public:
    World();
    virtual ~World();
    virtual void Tick(float);
    virtual bool HasLocalPlayer();
    ILocalPlayer* GetLocalPlayer();
    virtual bool IsAuthority();
    virtual void AddLocalPlayer(Player*, ILocalPlayer*);
    virtual void AddRemotePlayer(Player*);
    virtual void AddRemotePlayerWithId(uint32_t, Player*);
    virtual void RemovePlayer(Player*);
    virtual void Use(Player*, Actor*);
    virtual void Activate(Player*, IItem*);
    virtual void Reload(Player*);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(Player*, const std::string&);
    virtual void BuyItem(Player*, Actor*, IItem*, uint32_t);
    virtual void SellItem(Player*, Actor*, IItem*, uint32_t);
    virtual void Respawn(Player*);
    virtual void Teleport(Player*, const std::string&);
    virtual void Chat(Player*, const std::string&);
    virtual void FastTravel(Player*, const std::string&, const std::string&);
    virtual void SetPvPDesired(Player*, bool);
    virtual void SubmitDLCKey(Player*, const std::string&);
    virtual void SetCircuitInputs(Player*, const std::string&, uint32_t);
    virtual void SendAddItemEvent(Player*, IItem*, uint32_t);
    virtual void SendRemoveItemEvent(Player*, IItem*, uint32_t);
    virtual void SendLoadedAmmoEvent(Player*, IItem*, uint32_t);
    virtual void SendPickedUpEvent(Player*, const std::string&);
    virtual void EquipItem(Player*, uint8_t, IItem*);
    virtual void SetCurrentSlot(Player*, uint8_t);
    virtual void SendEquipItemEvent(Player*, uint8_t, IItem*);
    virtual void SendCurrentSlotEvent(Player*, uint8_t);
    virtual void SetCurrentQuest(Player*, IQuest*);
    virtual void SendSetCurrentQuestEvent(Player*, IQuest*);
    virtual void SendStartQuestEvent(Player*, IQuest*);
    virtual void SendAdvanceQuestToStateEvent(Player*, IQuest*, IQuestState*);
    virtual void SendCompleteQuestEvent(Player*, IQuest*);
    virtual void SendHealthUpdateEvent(Actor*, int32_t);
    virtual void SendManaUpdateEvent(Player*, int32_t);
    virtual void SendCountdownUpdateEvent(Player*, int32_t);
    virtual void SendPvPCountdownUpdateEvent(Player*, bool, int32_t);
    virtual void SendPvPEnableEvent(Player*, bool);
    virtual void SendStateEvent(Actor*, const std::string&, bool);
    virtual void SendTriggerEvent(Actor*, const std::string&, Actor*, bool);
    virtual void SendFireBulletsEvent(Actor*, IItem*, const Vector3&, uint32_t, float);
    virtual void SendDisplayEvent(Player*, const std::string&, const std::string&);
    virtual void SendNPCConversationStateEvent(Player*, Actor*, const std::string&);
    virtual void SendNPCConversationEndEvent(Player*);
    virtual void SendNPCShopEvent(Player*, Actor*);
    virtual void SendRespawnEvent(Player*, const Vector3&, const Rotation&);
    virtual void SendTeleportEvent(Actor*, const Vector3&, const Rotation&);
    virtual void SendRelativeTeleportEvent(Actor*, const Vector3&);
    virtual void SendReloadEvent(Player*, IItem*, IItem*, uint32_t);
    virtual void SendPlayerJoinedEvent(Player*);
    virtual void SendPlayerLeftEvent(Player*);
    virtual void SendPlayerItemEvent(Player*);
    virtual void SendActorSpawnEvent(Actor*);
    virtual void SendActorDestroyEvent(Actor*);
    virtual void SendExistingPlayerEvent(Player*, Player*);
    virtual void SendExistingActorEvent(Player*, Actor*);
    virtual void SendChatEvent(Player*, const std::string&);
    virtual void SendKillEvent(Player*, Actor*, IItem*);
    virtual void SendCircuitOutputEvent(Player*, const std::string&, uint32_t, const std::vector<std::vector<bool>>&);
    virtual void SendActorPositionEvents(Player*);
    virtual void SendRegionChangeEvent(Player*, const std::string&);
    virtual void SendLastHitByItemEvent(Player*, IItem*);
};

class ClientWorld : public World {
public:
    ActorRef<IPlayer> m_activePlayer;
    float m_timeUntilNextNetTick;

public:
    ClientWorld();
    virtual bool HasLocalPlayer();
    virtual bool IsAuthority();
    virtual void AddLocalPlayer(Player*, ILocalPlayer*);
    virtual void Tick(float);
    virtual void Use(Player*, Actor*);
    virtual void Activate(Player*, IItem*);
    virtual void Reload(Player*);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(Player*, const std::string&);
    virtual void BuyItem(Player*, Actor*, IItem*, uint32_t);
    virtual void SellItem(Player*, Actor*, IItem*, uint32_t);
    virtual void Respawn(Player*);
    virtual void Teleport(Player*, const std::string&);
    virtual void Chat(Player*, const std::string&);
    virtual void FastTravel(Player*, const std::string&, const std::string&);
    virtual void SetPvPDesired(Player*, bool);
    virtual void SubmitDLCKey(Player*, const std::string&);
    virtual void SetCircuitInputs(Player*, const std::string&, uint32_t);
    virtual void SendAddItemEvent(Player*, IItem*, uint32_t);
    virtual void SendRemoveItemEvent(Player*, IItem*, uint32_t);
    virtual void SendLoadedAmmoEvent(Player*, IItem*, uint32_t);
    virtual void SendPickedUpEvent(Player*, const std::string&);
    virtual void EquipItem(Player*, uint8_t, IItem*);
    virtual void SetCurrentSlot(Player*, uint8_t);
    virtual void SendEquipItemEvent(Player*, uint8_t, IItem*);
    virtual void SendCurrentSlotEvent(Player*, uint8_t);
    virtual void SetCurrentQuest(Player*, IQuest*);
    virtual void SendSetCurrentQuestEvent(Player*, IQuest*);
    virtual void SendStartQuestEvent(Player*, IQuest*);
    virtual void SendAdvanceQuestToStateEvent(Player*, IQuest*, IQuestState*);
    virtual void SendCompleteQuestEvent(Player*, IQuest*);
    virtual void SendHealthUpdateEvent(Actor*, int32_t);
    virtual void SendManaUpdateEvent(Player*, int32_t);
    virtual void SendCountdownUpdateEvent(Player*, int32_t);
    virtual void SendPvPCountdownUpdateEvent(Player*, bool, int32_t);
    virtual void SendPvPEnableEvent(Player*, bool);
    virtual void SendStateEvent(Actor*, const std::string&, bool);
    virtual void SendTriggerEvent(Actor*, const std::string&, Actor*, bool);
    virtual void SendFireBulletsEvent(Actor*, IItem*, const Vector3&, uint32_t, float);
    virtual void SendDisplayEvent(Player*, const std::string&, const std::string&);
    virtual void SendNPCConversationStateEvent(Player*, Actor*, const std::string&);
    virtual void SendNPCConversationEndEvent(Player*);
    virtual void SendNPCShopEvent(Player*, Actor*);
    virtual void SendRespawnEvent(Player*, const Vector3&, const Rotation&);
    virtual void SendTeleportEvent(Actor*, const Vector3&, const Rotation&);
    virtual void SendRelativeTeleportEvent(Actor*, const Vector3&);
    virtual void SendReloadEvent(Player*, IItem*, IItem*, uint32_t);
    virtual void SendPlayerJoinedEvent(Player*);
    virtual void SendPlayerLeftEvent(Player*);
    virtual void SendPlayerItemEvent(Player*);
    virtual void SendActorSpawnEvent(Actor*);
    virtual void SendActorDestroyEvent(Actor*);
    virtual void SendExistingPlayerEvent(Player*, Player*);
    virtual void SendExistingActorEvent(Player*, Actor*);
    virtual void SendChatEvent(Player*, const std::string&);
    virtual void SendKillEvent(Player*, Actor*, IItem*);
    virtual void SendCircuitOutputEvent(Player*, const std::string&, uint32_t, const std::vector<std::vector<bool>>&);
    virtual void SendActorPositionEvents(Player*);
    virtual void SendRegionChangeEvent(Player*, const std::string&);
    virtual void SendLastHitByItemEvent(Player*, IItem*);
};

class IInventory {
  public:
    virtual ~IInventory();
    virtual size_t GetCount();
    virtual class IItem * GetItem(size_t);
    virtual uint32_t GetItemCount(size_t);
    virtual uint32_t GetItemLoadedAmmo(size_t);
    virtual void Destroy();
};

class IActor {
  public:
    virtual ~IActor();
    virtual void * GetUE4Actor();
    virtual bool IsNPC();
    virtual bool IsPlayer();
    virtual class IPlayer * GetPlayerInterface();
    virtual void AddRef();
    virtual void Release();
    virtual void OnSpawnActor(class IUE4Actor *);
    virtual void OnDestroyActor();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool CanBeDamaged(class IActor *);
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(class IActor *, class IItem *, int32_t, enum DamageType);
    virtual void Tick(float);
    virtual bool CanUse(class IPlayer *);
    virtual void OnUse(class IPlayer *);
    virtual void OnHit(class IActor *, const struct Vector3 &, const struct Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual class IItem ** GetShopItems(size_t &);
    virtual void FreeShopItems(class IItem **);
    virtual int32_t GetBuyPriceForItem(class IItem *);
    virtual int32_t GetSellPriceForItem(class IItem *);
    virtual struct Vector3 GetLookPosition();
    virtual struct Rotation GetLookRotation();
    virtual class IActor * GetOwner();
};

class Actor : public IActor {
  public:
    size_t m_refs;
    uint32_t m_id;
    class IUE4Actor *m_target;
    class TimerSet *m_timers;
    std::string m_blueprintName;
    class ActorRef<IActor> m_owner;
    int32_t m_health;
    class std::map<std::basic_string<char>, bool, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, bool> > > m_states;
    float m_forwardMovementFraction;
    float m_strafeMovementFraction;
    struct Vector3 m_remotePosition;
    struct Vector3 m_remoteVelocity;
    struct Rotation m_remoteRotation;
    float m_remoteLocationBlendFactor;
    class Spawner *m_spawner;

    virtual void OnKilled(class IActor *, class IItem *);
    virtual void OnTargetKilled(class IActor *, class IItem *);
  public:
    Actor(const std::string &);
    virtual ~Actor();
    virtual bool IsValid() const;
    virtual void * GetUE4Actor();
    virtual void AddRef();
    virtual void Release();
    void RemoveFromWorld();
    virtual void OnSpawnActor(class IUE4Actor *);
    virtual void OnDestroyActor();
    virtual std::string GetDeathMessage();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool IsNPC();
    virtual bool IsProjectile();
    virtual bool IsPlayer();
    virtual class IPlayer * GetPlayerInterface();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    uint32_t GetId() const;
    void SetId(uint32_t);
    struct Vector3 GetPosition();
    struct Vector3 GetProjectilePosition();
    virtual struct Vector3 GetLookPosition();
    struct Rotation GetRotation();
    virtual struct Rotation GetLookRotation();
    struct Vector3 GetVelocity();
    float GetForwardMovementFraction() const;
    float GetStrafeMovementFraction() const;
    bool IsOnGround();
    void SetPosition(const struct Vector3 &);
    void SetRotation(const struct Rotation &);
    void SetVelocity(const struct Vector3 &);
    void SetForwardAndStrafeMovement(float, float);
    void SetRemotePositionAndRotation(const struct Vector3 &, const struct Rotation &);
    void InterpolateRemotePosition(float);
    virtual class IActor * GetOwner();
    void LocalRespawn(const struct Vector3 &, const struct Rotation &);
    bool MoveToLocation(const struct Vector3 &);
    bool MoveToRandomLocationInRadius(float);
    bool MoveToActor(class IActor *);
    bool GetState(const std::string &);
    virtual void UpdateState(const std::string &, bool);
    virtual void TriggerEvent(const std::string &, class IActor *, bool);
    const class std::map<std::basic_string<char>, bool, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, bool> > > & GetStates();
    class IActor * LineTraceTo(const struct Vector3 &);
    void FireBullets(class IItem *, int32_t, enum DamageType, float, uint32_t, float);
    void FireBullets(class IItem *, int32_t, enum DamageType, const struct Vector3 &, float, uint32_t, float);
    virtual bool CanBeDamaged(class IActor *);
    virtual float GetMaximumDamageDistance();
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(class IActor *, class IItem *, int32_t, enum DamageType);
    void PerformSetHealth(int32_t);
    virtual void Tick(float);
    virtual bool CanUse(class IPlayer *);
    virtual void OnUse(class IPlayer *);
    virtual void PerformUse(class IPlayer *);
    virtual void OnHit(class IActor *, const struct Vector3 &, const struct Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual class IItem ** GetShopItems(size_t &);
    virtual class std::vector<IItem*, std::allocator<IItem*> > GetShopItems();
    virtual void FreeShopItems(class IItem **);
    virtual class std::vector<IItem*, std::allocator<IItem*> > GetValidBuyItems();
    virtual float GetShopBuyPriceMultiplier();
    virtual float GetShopSellPriceMultiplier();
    virtual int32_t GetBuyPriceForItem(class IItem *);
    virtual int32_t GetSellPriceForItem(class IItem *);
    void SetSpawner(class Spawner *);
    void AddTimer(const std::string &, float, const class std::function<void ()> &);
    void AddTimerWithContext(const std::string &, float, const class std::function<void (Actor *)> &);
    void AddRecurringTimer(const std::string &, float, const class std::function<void ()> &);
    void AddRecurringTimerWithContext(const std::string &, float, const class std::function<void (Actor *)> &);
    void CancelTimer(const std::string &);
    void PerformReloadNotification(uint32_t);
};

class IPlayer {
  public:
    virtual IActor* GetActorInterface() = 0;
    virtual void AddRef() = 0;
    virtual void Release() = 0;
    virtual bool IsLocalPlayer() const = 0;
    virtual ILocalPlayer* GetLocalPlayer() const = 0;
    virtual const char* GetPlayerName() = 0;
    virtual const char* GetTeamName() = 0;
    virtual uint8_t GetAvatarIndex() = 0;
    virtual const uint32_t* GetColors() = 0;
    virtual bool IsPvPDesired() = 0;
    virtual void SetPvPDesired(bool) = 0;
    virtual IInventory* GetInventory() = 0;
    virtual uint32_t GetItemCount(IItem*) = 0;
    virtual uint32_t GetLoadedAmmo(IItem*) = 0;
    virtual bool AddItem(IItem*, uint32_t, bool) = 0;
    virtual bool RemoveItem(IItem*, uint32_t) = 0;
    virtual bool AddLoadedAmmo(IItem*, IItem*, uint32_t) = 0;
    virtual bool RemoveLoadedAmmo(IItem*, uint32_t) = 0;
    virtual IItem* GetItemForSlot(size_t) = 0;
    virtual void EquipItem(size_t, IItem*) = 0;
    virtual size_t GetCurrentSlot() = 0;
    virtual void SetCurrentSlot(size_t) = 0;
    virtual IItem* GetCurrentItem() = 0;
    virtual int32_t GetMana() = 0;
    virtual bool UseMana(int32_t) = 0;
    virtual void SetItemCooldown(IItem*, float, bool) = 0;
    virtual bool IsItemOnCooldown(IItem*) = 0;
    virtual float GetItemCooldown(IItem*) = 0;
    virtual bool HasPickedUp(const char*) = 0;
    virtual void MarkAsPickedUp(const char*) = 0;
    virtual IQuest** GetQuestList(size_t*) = 0;
    virtual void FreeQuestList(IQuest**) = 0;
    virtual IQuest* GetCurrentQuest() = 0;
    virtual void SetCurrentQuest(IQuest*) = 0;
    virtual PlayerQuestState GetStateForQuest(IQuest*) = 0;
    virtual void StartQuest(IQuest*) = 0;
    virtual void AdvanceQuestToState(IQuest*, IQuestState*) = 0;
    virtual void CompleteQuest(IQuest*) = 0;
    virtual bool IsQuestStarted(IQuest*) = 0;
    virtual bool IsQuestCompleted(IQuest*) = 0;
    virtual void EnterAIZone(const char*) = 0;
    virtual void ExitAIZone(const char*) = 0;
    virtual void UpdateCountdown(int32_t) = 0;
    virtual bool CanReload() = 0;
    virtual void RequestReload() = 0;
    virtual float GetWalkingSpeed() = 0;
    virtual float GetSprintMultiplier() = 0;
    virtual float GetJumpSpeed() = 0;
    virtual float GetJumpHoldTime() = 0;
    virtual bool CanJump() = 0;
    virtual void SetJumpState(bool) = 0;
    virtual void SetSprintState(bool) = 0;
    virtual void SetFireRequestState(bool) = 0;
    virtual void TransitionToNPCState(const char*) = 0;
    virtual void BuyItem(IActor*, IItem*, uint32_t) = 0;
    virtual void SellItem(IActor*, IItem*, uint32_t) = 0;
    virtual void EnterRegion(const char*) = 0;
    virtual void Respawn() = 0;
    virtual void Teleport(const char*) = 0;
    virtual void Chat(const char*) = 0;
    virtual IFastTravel* GetFastTravelDestinations(const char*) = 0;
    virtual void FastTravel(const char*, const char*) = 0;
    virtual void MarkAsAchieved(IAchievement*) = 0;
    virtual bool HasAchieved(IAchievement*) = 0;
    virtual void SubmitDLCKey(const char*) = 0;
    virtual uint32_t GetCircuitInputs(const char*) = 0;
    virtual void SetCircuitInputs(const char*, uint32_t) = 0;
    virtual void GetCircuitOutputs(const char*, bool*, size_t) = 0;
};

class Player : public Actor, public IPlayer {
public:
    uint32_t m_characterId;
    const char* m_playerName;
    const char* m_teamName;
    uint8_t m_avatarIndex;
    uint32_t m_colors[4];
    std::map<IItem*, ItemAndCount> m_inventory;
    std::set<std::string> m_pickups;
    std::map<IItem*, float> m_cooldowns;
    std::map<std::string, unsigned int> m_circuitInputs;
    std::map<std::string, std::vector<std::allocator<bool>>> m_circuitOutputs;
    bool m_admin;
    bool m_pvpEnabled;
    bool m_pvpDesired;
    float m_pvpChangeTimer;
    int32_t m_pvpChangeReportedTimer;
    bool m_changingServerRegion;
    const char* m_currentRegion;
    const char* m_changeRegionDestination;
    std::set<std::string> m_aiZones;
    int32_t m_mana;
    float m_manaRegenTimer;
    float m_healthRegenCooldown;
    float m_healthRegenTimer;
    int32_t m_countdown;
    Vector3 m_remoteLookPosition;
    Rotation m_remoteLookRotation;
    IItem *m_equipped[10];
    size_t m_currentSlot;
    std::map<IQuest*, PlayerQuestState> m_questStates;
    IQuest *m_currentQuest;
    float m_walkingSpeed;
    float m_jumpSpeed;
    float m_jumpHoldTime;
    ActorRef<NPC> m_currentNPC;
    const char* m_currentNPCState;
    ILocalPlayer *m_localPlayer;
    WriteStream *m_eventsToSend;
    bool m_itemsUpdated;
    float m_itemSyncTimer;
    uint32_t m_chatMessageCounter;
    float m_chatFloodDecayTimer;
    IItem *m_lastHitByItem;
    float m_lastHitItemTimeLeft;
    float m_circuitStateCooldownTimer;

protected:
    virtual void OnKilled(IActor *, IItem *);

public:
    Player(bool);
    virtual ~Player(void);
    virtual bool IsPlayer(void);
    virtual IPlayer * GetPlayerInterface(void);
    virtual IActor * GetActorInterface(void);
    virtual bool CanBeDamaged(IActor *);
    virtual bool IsCharacter(void);
    virtual bool ShouldSendPositionUpdates(void);
    virtual bool ShouldReceivePositionUpdates(void);
    virtual void Tick(float);
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    virtual void OnDestroyActor(void);
    void OnKillEvent(IPlayer *, IActor *, IItem *);
    virtual Vector3 GetLookPosition(void);
    virtual Rotation GetLookRotation(void);
    void SetRemoteLookPosition(const Vector3 &);
    void SetRemoteLookRotation(const Rotation &);
    virtual bool CanJump(void);
    virtual bool IsLocalPlayer(void) const;
    virtual ILocalPlayer * GetLocalPlayer(void) const;
    void InitLocalPlayer(ILocalPlayer *);
    bool IsAdmin(void) const;
    void SetPlayerName(const std::string &);
    void SetTeamName(const std::string &);
    void SetAvatarIndex(uint8_t);
    void SetColors(const uint32_t *);
    void SetCharacterId(uint32_t);
    virtual bool IsPvPEnabled(void);
    virtual bool IsPvPDesired(void);
    virtual void SetPvPDesired(bool);
    void PerformSetPvPEnabled(bool);
    void PerformSetPvPDesired(bool);
    void PerformUpdatePvPCountdown(bool, int32_t);
    virtual void UpdateState(const std::string &, bool);
    virtual const char * GetPlayerName(void);
    virtual const char * GetTeamName(void);
    virtual uint8_t GetAvatarIndex(void);
    virtual const uint32_t * GetColors(void);
    uint32_t GetCharacterId(void) const;
    const std::map<IItem*, ItemAndCount> & GetItemList(void) const;
    virtual IInventory * GetInventory(void);
    virtual uint32_t GetItemCount(IItem *);
    virtual uint32_t GetLoadedAmmo(IItem *);
    virtual bool AddItem(IItem *, uint32_t, bool);
    virtual bool RemoveItem(IItem *, uint32_t);
    bool PerformAddItem(IItem *, uint32_t, bool);
    bool PerformRemoveItem(IItem *, uint32_t);
    virtual bool AddLoadedAmmo(IItem *, IItem *, uint32_t);
    virtual bool RemoveLoadedAmmo(IItem *, uint32_t);
    void PerformSetLoadedAmmo(IItem *, uint32_t);
    virtual IItem * GetItemForSlot(size_t);
    virtual void EquipItem(size_t, IItem *);
    void PerformEquipItem(size_t, IItem *);
    virtual size_t GetCurrentSlot(void);
    virtual IItem * GetCurrentItem(void);
    virtual void SetCurrentSlot(size_t);
    void PerformSetCurrentSlot(size_t);
    void SetRemoteItem(IItem *);
    virtual int32_t GetMana(void);
    virtual bool UseMana(int32_t);
    void PerformSetMana(int32_t);
    virtual void SetItemCooldown(IItem *, float, bool);
    virtual bool IsItemOnCooldown(IItem *);
    virtual float GetItemCooldown(IItem *);
    virtual bool HasPickedUp(const char *);
    virtual void MarkAsPickedUp(const char *);
    void PerformMarkAsPickedUp(const std::string &);
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
    void PerformSetCurrentQuest(IQuest *);
    void PerformStartQuest(IQuest *);
    void PerformAdvanceQuestToState(IQuest *, IQuestState *);
    void PerformCompleteQuest(IQuest *);
    void SetInitialQuestStates(const std::map<std::string, QuestStateInfo> &, const std::string &);
    void SetInitialItemState(const std::map<std::string, ItemCountInfo> &, const std::vector<std::string> &, uint8_t);
    void SetInitialPickupState(const std::set<std::string> &);
    virtual void EnterAIZone(const char *);
    virtual void ExitAIZone(const char *);
    virtual void UpdateCountdown(int32_t);
    void PerformUpdateCountdown(int32_t);
    virtual void TriggerEvent(const std::string &, IActor *, bool);
    virtual bool CanReload(void);
    virtual void RequestReload(void);
    void PerformRequestReload(void);
    virtual float GetWalkingSpeed(void);
    virtual float GetSprintMultiplier(void);
    virtual float GetJumpSpeed(void);
    virtual float GetJumpHoldTime(void);
    virtual void SetJumpState(bool);
    virtual void SetSprintState(bool);
    virtual void SetFireRequestState(bool);
    void SetCurrentNPCState(NPC *, const std::string &);
    void EndNPCConversation(void);
    void EnterNPCShop(NPC *);
    NPC * GetCurrentNPC(void) const;
    const std::string & GetCurrentNPCState(void) const;
    virtual void TransitionToNPCState(const char *);
    void PerformTransitionToNPCState(const std::string &);
    virtual void BuyItem(IActor *, IItem *, uint32_t);
    void PerformBuyItem(IActor *, IItem *, uint32_t);
    virtual void SellItem(IActor *, IItem *, uint32_t);
    void PerformSellItem(IActor *, IItem *, uint32_t);
    virtual void EnterRegion(const char *);
    bool IsChangingRegion(void) const;
    const std::string & GetChangeRegionDestination(void) const;
    void PerformEnterRegion(const std::string &);
    LocationAndRotation GetSpawnLocation(void);
    virtual void Respawn(void);
    void PerformRespawn(void);
    void PerformRespawnAtLocation(const Vector3 &, const Rotation &);
    virtual void Teleport(const char *);
    void PerformTeleport(const std::string &);
    virtual void SendEvent(const WriteStream &);
    virtual void WriteAllEvents(WriteStream &);
    void SyncItems(void);
    virtual void Chat(const char *);
    void PerformChat(const std::string &);
    void ReceiveChat(Player *, const std::string &);
    virtual IFastTravel * GetFastTravelDestinations(const char *);
    virtual void FastTravel(const char *, const char *);
    void PerformFastTravel(const std::string &, const std::string &);
    void OnTravelComplete(const std::string &);
    IItem * GetLastHitByItem(void) const;
    void PerformSetLastHitByItem(IItem *);
    virtual void MarkAsAchieved(IAchievement *);
    virtual bool HasAchieved(IAchievement *);
    virtual void SubmitDLCKey(const char *);
    void PerformSubmitDLCKey(const std::string &);
    virtual uint32_t GetCircuitInputs(const char *);
    virtual void SetCircuitInputs(const char *, uint32_t);
    void PerformSetCircuitInputs(const std::string &, uint32_t);
    virtual void GetCircuitOutputs(const char *, bool *, size_t);
    void PerformSetCircuitOutputs(const std::string &, std::vector<std::allocator<bool>>);
    void InitCircuitStates(void);
};

class AIActor : public Actor {
  protected:
    class std::map<std::string, AIState*, std::less<std::string>, std::allocator<std::pair<std::string const, AIState*> > > m_states;
    class AIState *m_initialState;
    class AIState *m_currentState;
    class ActorRef<Actor> m_target;

    void AddInitialState(const std::string &, class AIState *);
    void AddState(const std::string &, class AIState *);
  public:
    AIActor(const std::string &);
    virtual ~AIActor();
    virtual bool IsCharacter();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    class Actor * GetTarget() const;
    virtual bool ShouldTargetPlayer(class Player *);
    virtual bool ShouldAttackFromRange() const;
    virtual float GetRangedAttackDistance() const;
    virtual bool ShouldWander() const;
    virtual bool ShouldMove() const;
    virtual bool ShouldAttack() const;
    virtual bool ShouldAttackMultipleTargets() const;
    virtual void Tick(float);
    virtual void OnAIMoveComplete();
    class AIState * GetStateByName(const std::string &);
    void TransitionToState(const std::string &, class Actor *);
    virtual void TransitionToState(class AIState *, class Actor *);
};


class Enemy : public AIActor {
  protected:

    virtual void OnKilled(class IActor *, class IItem *);
  public:
    Enemy(const std::string &);
    virtual bool CanBeDamaged(class IActor *);
    virtual float GetMaximumDamageDistance();
    int32_t GetAttackDamage();
    virtual enum DamageType GetAttackDamageType();
    virtual class IItem * GetAttackItem();
    virtual float GetAggressionRadius();
    virtual float GetAttackTime();
    virtual float GetAttackHitTime();
    virtual void OnPrepareAttack(class Actor *);
    virtual void OnEndAttack();
    virtual void Attack(class Actor *);
    virtual enum EnemyRank GetRank() const;
    virtual struct Rotation GetLookRotation();
    virtual void Damage(class IActor *, class IItem *, int32_t, enum DamageType);
};

class Bear : public Enemy {
  protected:
    int32_t m_attacksLeftInPosition;
    class ActorRef<BearChest> m_chest;

    void Init();
    virtual void OnKilled(class IActor *, class IItem *);
    virtual void OnTargetKilled(class IActor *, class IItem *);
  public:
    Bear();
    Bear(class BearChest *, const std::string &);
    virtual ~Bear();
    virtual float GetMaximumDamageDistance();
    virtual float GetAggressionRadius();
    virtual int32_t GetAttackDamage();
    virtual bool CanBeArmed();
    virtual void OnPrepareAttack(class Actor *);
    virtual void OnEndAttack();
    void AttackForChest(class IPlayer *);
    void EndChestDefense();
    virtual int32_t GetMaxHealth();
    virtual const char * GetDisplayName();
    virtual std::string GetDeathMessage();
};

class BearChest : public Actor {
  private:
    class std::vector<ActorRef<Bear>, std::allocator<ActorRef<Bear> > > m_bears;
    class std::map<ActorRef<IPlayer>, float, std::less<ActorRef<IPlayer> >, std::allocator<std::pair<ActorRef<IPlayer> const, float> > > m_playerTimeLeft;

    void UpdatePlayerAttacks();
    float GetMinimumTimeRemaining();
  public:
    BearChest();
    virtual bool CanUse(class IPlayer *);
    virtual void PerformUse(class IPlayer *);
    void AddBear(class Bear *);
    void RemoveBear(class Bear *);
    virtual void Tick(float);
    bool IsEliteStage();
    bool IsArmedStage();
    size_t GetQuestPlayerCount() const;
};

class AngryBear : public Bear {
  private:
    void InitAngryBear();
  public:
    AngryBear();
    AngryBear(class BearChest *, const std::string &);
    virtual float GetMaximumDamageDistance();
    virtual float GetAggressionRadius();
    virtual int32_t GetMaxHealth();
    virtual int32_t GetAttackDamage();
    virtual const char * GetDisplayName();
    virtual std::string GetDeathMessage();
    virtual bool IsElite();
    virtual bool CanBeArmed();
};

#endif // LIBGAMELOGIC_H

