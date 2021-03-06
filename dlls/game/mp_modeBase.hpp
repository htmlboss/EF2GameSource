//-----------------------------------------------------------------------------
//
//  $Logfile:: /Code/DLLs/game/multiplayerArena.h                             $
// $Revision:: 38                                                             $
//   $Author:: Steven                                                         $
//     $Date:: 7/23/02 3:55p                                                  $
//
// Copyright (C) 2002 by Ritual Entertainment, Inc.
// All rights reserved.
//
// This source may not be distributed and/or modified without
// expressly written permission by Ritual Entertainment, Inc.
//
// 
// Description:
//

#ifndef __MP_MODEBASE_HPP__
#define __MP_MODEBASE_HPP__

#include "g_local.h"          // common game stuff
#include "player.h"           // for Player
#include "item.h"
#include "PlayerStart.h"      // for PlayerDeathmatchStart
#include "container.h"        // for Container
#include "str.h"              // for str
#include "mp_team.hpp"
#include "mp_shared.hpp"
#include "equipment.h"

class MultiplayerPlayerGameData
{
public:
	int					_numDeaths;
	int					_numKills;
	int					_points;

	int					_entnum;

	bool				_playing;

	float				_nextHitSoundTime;

	float				_startTime;

	Team				*_currentTeam;

	int					_lastKilledByPlayer;
	int					_lastKillerOfPlayer;

	int					_lastKilledByPlayerMOD;
	int					_lastKillerOfPlayerMOD;

	int					_lastPlace;
	bool				_lastTied;

						MultiplayerPlayerGameData();
	void				init( void );
	void				reset( void );
};

//-----------------------------------------------------------------------
// MultiplayerModeBase --  Abstract base class providing common functionality
//                      to all multiplayer game types.  Tracks players 
//                      entering the arena, starting the match, etc.
//-----------------------------------------------------------------------
class MultiplayerModeBase : public Class
	{
	protected:
		static const float					_defaultStartinghealth;
		static const int					_defaultPointsPerKill;
		static const int					_defaultPointsPerTakenAwayForSuicide;
		static const float					_spectatorMoveSpeedModifier;

		Container<Player *>					_playerList;
		Container<PlayerDeathmatchStart *>	_spawnpointList;
		Container<PlayerDeathmatchStart *>	_unusedSpawnpointList;
		Container<str>                      _weaponList;
		Container<SimpleAmmoType *>         _ammoList;
		unsigned int						_activePlayers ;
		unsigned int						_spawncounter ;
		unsigned int                        _startingHealth ;
		bool								_fightInProgress ;

		str									_startingWeaponName;

		MultiplayerPlayerGameData			*_playerGameData;
		unsigned int						_maxPlayers;
		int									_pointLimit;

		float								_matchStartTime;
		float								_gameStartTime;
		float								_timeLimit;

		bool								_gameStarted;
		int									_lastTimeRemaining;

		int									_spectatorIconIndex;

		int									_warmUpTextIndex;
		int									_waitingForMinPlayersTextIndex;
		int									_playingTextIndex;

		int									_lastHighestPoints;

		bool								_played5MinWarning;
		bool								_played2MinWarning;
		bool								_played1MinWarning;

		// Abstract constructor
											MultiplayerModeBase();
		
		int									getNumSpawnpoints( void );
		Entity *							getSpawnpointbyIndex( int index );
		Entity *							getRandomSpawnpoint( bool useCounter = false );

		int									getNumNamedSpawnpoints( const str &spawnpointType );
		Entity *							getNamedSpawnpointbyIndex( const str &spawnpointType, int index );
		Entity *							getRandomNamedSpawnpoint( const str &spawnpointType );
		Entity *							getFarthestNamedSpawnpoint( const Vector &origin, const str &spawnpointType );
		
		virtual void                        _endMatch();
		virtual void                        _beginMatch();
		virtual void                        _giveInitialConditions(Player* player);

		void								handleKill( Player *killedPlayer, Player *attackingPlayer, Entity *inflictor, int meansOfDeath, bool goodKill );

		Player *							getPlayer( unsigned entnum );

	public:
		CLASS_PROTOTYPE( MultiplayerModeBase );
		virtual	~MultiplayerModeBase();

		virtual void						init( int maxPlayers );
		virtual void						initItems( void );
		virtual void						start( void ) {};
		virtual bool						shouldKeepItem( MultiplayerItem *item ) { Q_UNUSED(item); return false; }
		virtual bool						shouldKeepNormalItem( Item *item );
		virtual void						itemKept( MultiplayerItem *item ) { Q_UNUSED(item); };
		virtual void						update( float frameTime );

		virtual void						matchOver( void ) {};
		virtual void						declareWinner( void );

		void								getSpawnpoints( void );
		virtual Entity *					getSpawnPoint( Player *player );

		int									findPlayer( const Player *player );

		virtual bool						canGivePlayerItem( int entnum, const str &itemName ) { Q_UNUSED(entnum); Q_UNUSED(itemName); return true; }

		// Queries
		virtual bool                        isEndOfMatch( void );
		bool								isFightInProgress()                             { return _fightInProgress ; }

		// Gets
		//inline int							getID( void )                                   { return _id; }
		inline unsigned int                 getStartingHealth( void )                       { return _startingHealth ; }
		inline unsigned int                 getMaxPlayers( void )                           { return _maxPlayers ; }
		inline unsigned int                 getActivePlayers( void )                        { return _activePlayers ; }

		inline int							getPointLimit( void )							{ return _pointLimit ; }
		inline unsigned int					getTimeLimit( void )							{ return (unsigned int) _timeLimit ; }

		int									getPoints( Player *player );
		int									getKills( Player *player );
		int									getDeaths( Player *player );
		Team*								getPlayersTeam( const Player *player );
		virtual int							getTeamPoints( Player *player ) { Q_UNUSED(player); return 0; }
		virtual int							getTeamPoints( const str & teamName ) { Q_UNUSED(teamName); return 0; }
		virtual void						addTeamPoints( const str & teamName, int points ) { Q_UNUSED(teamName); Q_UNUSED(points); return; }

		virtual int							getStat( Player *player, int statNum, int value );
		virtual int							getIcon( Player *player, int statNum, int value );
		virtual int							getScoreIcon( Player *player, int index, int value ) { Q_UNUSED(player); Q_UNUSED(index); return value; }
		virtual int							getInfoIcon( Player *player ) { Q_UNUSED(player); return 0; }

		virtual Player *					getLastKilledByPlayer( Player *player, int *meansOfDeath );
		virtual Player *					getLastKillerOfPlayer( Player *player, int *meansOfDeath );

		int									comparePlayerScore( MultiplayerPlayerGameData &player1Data, MultiplayerPlayerGameData &player2Data );
		int									getPlace( Player *player, bool *tied = NULL );
		
		// Sets
		//void								setID( int id );
		void                                setStartingHealth( unsigned int startingHealth) { _startingHealth = startingHealth ; }
		void								setPointLimit( int pointLimit ) { _pointLimit = pointLimit; }
		void								setTimeLimit( float timeLimit )					{ _timeLimit = timeLimit; }

		// Arena wide functions
		virtual void						resetSpawnpoints( void );
		virtual void						BeginMatch( void );
		virtual void                        EndMatch( void );
		virtual bool						inMatch( void );

		// Player specific functions
		virtual void						ActivatePlayer( Player *player );
		virtual void						AddPlayer( Player *player );
		virtual void						RemovePlayer( Player *player );
		bool								needToAddPlayer( Player *player );

		virtual bool						canJoinTeam( Player *player, const str &teamName );
		virtual void						joinTeam( Player *player, const str &teamName );

		virtual bool						canPickup( Player *player, MultiplayerItemType itemType, const char *item_name ) { Q_UNUSED(player); Q_UNUSED(itemType); Q_UNUSED(item_name); return true; }
		virtual void						pickedupItem( Player *player, MultiplayerItemType itemType, const char *itemName ) { Q_UNUSED(player); Q_UNUSED(itemType); Q_UNUSED(itemName); };

		virtual void						applySpeedModifiers( Player *player, int *moveSpeed );
		virtual void						applyJumpModifiers( Player *player, int *jumpSpeed ) { Q_UNUSED(player); Q_UNUSED(jumpSpeed); };
		virtual void						applyAirAccelerationModifiers( Player *player, int *airAcceleration ) { Q_UNUSED(player); Q_UNUSED(airAcceleration); };

		virtual void						setupMultiplayerUI( Player *player );

		virtual void						playerKilled( Player *killedPlayer, Player *attackingPlayer, Entity *inflictor, int meansOfDeath );
		virtual float						playerDamaged( Player *damagedPlayer, Player *attackingPlayer, float damage, int meansOfDeath );
		virtual void						playerTookDamage( Player *damagedPlayer, Player *attackingPlayer, float damage, int meansOfDeath );
		virtual void						playerFired( Player *attackingPlayer ) { Q_UNUSED(attackingPlayer); };
		virtual void						obituary( Player *killedPlayer, Player *attackingPlayer, int meansOfDeath );
		virtual void						playerDead( Player *player );

		virtual void						itemTouched( Player *player, MultiplayerItem *item ) { Q_UNUSED(player); Q_UNUSED(item); };
		virtual void						itemDestroyed( Player *player, MultiplayerItem *item ) { Q_UNUSED(player); Q_UNUSED(item); };
		virtual float						itemDamaged( MultiplayerItem *item, Player *attackingPlayer, float damage, int meansOfDeath ) { Q_UNUSED(item); Q_UNUSED(attackingPlayer); Q_UNUSED(meansOfDeath); return damage; }
		virtual void						itemUsed( Entity *entity, MultiplayerItem *item ) { Q_UNUSED(entity); Q_UNUSED(item); };

		virtual void						playerUsed( Player *usedPlayer, Player *usingPlayer, Equipment *equipment ) { Q_UNUSED(usedPlayer); Q_UNUSED(usingPlayer); Q_UNUSED(equipment); };

		virtual void						score( const Player *player );

		virtual void						respawnPlayer( Player *player ) { Q_UNUSED(player); };

		virtual void						playerEventNotification( const char *eventName, const char *eventItemName, Player *eventPlayer ) { Q_UNUSED(eventName); Q_UNUSED(eventItemName); Q_UNUSED(eventPlayer); };

		// Utility functions
		void								AddStartingWeapon(const str& weaponViewmodel);
		void								SetStartingWeapon( const str& weaponName );

		virtual void						addPoints( int entnum, int points );

		void								readMultiplayerConfig( const char *configName );
		virtual bool						parseConfigToken( const char *key, Script *buffer );

		bool								shouldStartMatch( void );
		void								startMatch( void );
		void								endMatch( void );
		void								restartMatch( void );

		virtual bool						checkGameType( const char *rule ) { Q_UNUSED(rule); return false; }
		virtual bool						doesPlayerHaveItem( Player *player, const char *itemName ) { Q_UNUSED(player); Q_UNUSED(itemName); return false; }

		virtual void						playerCommand( Player *player, const char *command, const char *parm ) { Q_UNUSED(player); Q_UNUSED(command); Q_UNUSED(parm); };

		virtual void						teamPointsChanged( Team *team, int oldPoints, int newPoints ) { Q_UNUSED(team); Q_UNUSED(oldPoints); Q_UNUSED(newPoints); };

		virtual bool						checkRule( const char *rule, bool defaultValue, Player *player = NULL ) { Q_UNUSED(rule); Q_UNUSED(player); return defaultValue; }

		virtual int							getHighestPoints( void );

		virtual bool						isValidPlayerModel( Player *player, str modelToUse, bool defaultValue ) { Q_UNUSED(player); Q_UNUSED(modelToUse); return defaultValue; }
		virtual str							getDefaultPlayerModel( Player *player, str modelName ) { Q_UNUSED(player); return modelName; }

		virtual void						playerChangedModel( Player * ) {};

		virtual bool						skipWeaponReloads( void ) { return false; }
	};

#endif // __MP_MODEBASE_HPP__
