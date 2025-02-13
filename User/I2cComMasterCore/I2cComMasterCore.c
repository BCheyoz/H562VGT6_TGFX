/*
 * I2cComMasterCore.c
 *
 *  Created on: 28 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 16 Nov. 2022
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 05/10/2021 : Added by Jp to RMD_Firmware
 *-> 26/10/2021 : Added by Jp to HII_Manta_App
 *-> 02/09/2022 : Added by Jp to Himalaya1_Carte6 (App)
 *-> 20/10/2022 : Merge by Jp to HII_Manta_App
 *-> 08/11/2022 : Merge by Jp to HII_CarteMere_App
 *
 *  Pour intégrer facilement cette Librairie "I2cComMaster" dans un nouveau Projet :
 *   -> Suivre les indications dans "I2cComMasterConf.h"
 *
 */

#include "I2cComMasterCore.h"	// Pour accès à nos propres déclarations publiques
#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

/******************************************************************************/

#define I2CCM_ADD_IGNORE_QRY_RST_ON_PERIF_RST	5
#define I2CCM_MAX_IGNORE_QRY_RST_ON_PERIF_OK	5
#define I2CCM_MAX_ALLOWED_RESTART_PERIF_CT		(UINT8_MAX / I2CCM_ADD_IGNORE_QRY_RST_ON_PERIF_RST)
#define I2CCM_REFRAME_PTR_WITHIN_OBJ_BASE_END(ptr,BaseObj,EndObj)				if( ((BaseObj) > (ptr)) || ((EndObj) <= (ptr)) ) { ptr = (BaseObj);}
#define I2CCM_REFRAME_PTR_WITHIN_TYPED_OBJ_BASE_END(ptr,BaseObj,EndObj,Cast)	if( ((BaseObj) > (ptr)) || (((void*)(EndObj)) <= ((void*)(ptr))) ) { ptr = ((Cast)BaseObj);}

void i2cComMasterHandleComResult(int8_t* pNbComResultOK, int32_t comResult);
#define I2CCM_MAX_NB_COM_RESULT_OK	3
#define I2CCM_NB_COM_RESULT_NO_ERR	0
#define I2CCM_MIN_NB_COM_RESULT_ERR	-99

/******************************************************************************/

uint8_t i2cComMasterHandleDeviceList(I2CCM_DevManager* pI2cDevManager, I2CCM_DevManagerParams* pI2cDevManParams)
{
	uint8_t mayReInitPerif = 0;
	if(0 == pI2cDevManager) return mayReInitPerif;
	if(0 == pI2cDevManParams) return mayReInitPerif;

	// Sécurisation des Pointeurs tournants :
	I2CCM_REFRAME_PTR_WITHIN_OBJ_BASE_END(pI2cDevManager->pManagedDevice, pI2cDevManParams->pDevListBase,  pI2cDevManParams->pDevListEnd);
	I2CCM_REFRAME_PTR_WITHIN_OBJ_BASE_END(pI2cDevManager->pNxtInitDevice, pI2cDevManParams->pInitListBase, pI2cDevManParams->pInitListEnd);
	I2CCM_Manage_Device *pManagedDevice = pI2cDevManager->pManagedDevice;

	// Etape 1 : Traitement des "Slots" libres :
	I2CCM_DevInitParams   *pNewInitDevice  = 0;
	if(0 == pManagedDevice->pInitDevice) // Slot de Gestion libre :
	{
		// Rechercher un Groupe qui n'a pas encore de représentant :
		int16_t newDeviceGroup = I2CCM_DEV_GROUP_UNKNOWN; // Par défaut : ne pas charger
		void* startPtr = pI2cDevManager->pNxtInitDevice; // OK, Le ReFrame vient d'être fait plus haut
		if(startPtr < (void*)(pI2cDevManParams->pInitListEnd))	// Vérifie tout de même que la Liste ne soit pas vide !
		{
			do
			{
				newDeviceGroup = pI2cDevManager->pNxtInitDevice->deviceGroup;
				for(I2CCM_Manage_Device* tmpDevice = pI2cDevManParams->pDevListBase; tmpDevice < pI2cDevManParams->pDevListEnd; tmpDevice++)
				{
					if(0 == tmpDevice->pInitDevice) continue; // Si pas Initialisé => Vérifier le suivant ...
					if(newDeviceGroup != tmpDevice->pInitDevice->deviceGroup) continue; // Si pas représenté => Vérifier le suivant ...

					// Ce Groupe est déjà représenté :
					newDeviceGroup = I2CCM_DEV_GROUP_UNKNOWN; // Ne pas charger cet Item
					break;
				}

				// Si ce Groupe n'a pas encore de représentant :
				if(I2CCM_DEV_GROUP_UNKNOWN != newDeviceGroup)
				{
					pNewInitDevice = (pI2cDevManager->pNxtInitDevice)++;   // Charger cet InitItem
					break;
				}

				(pI2cDevManager->pNxtInitDevice)++; // Pointer l'élément suivant
				I2CCM_REFRAME_PTR_WITHIN_OBJ_BASE_END(pI2cDevManager->pNxtInitDevice, pI2cDevManParams->pInitListBase, pI2cDevManParams->pInitListEnd);
			} while(pI2cDevManager->pNxtInitDevice != startPtr);	// Quitter la boucle de Recherche si on est revenu au point de départ !
		}
	}
	else // Si Slot déjà  Chargé : Gestion des demandes spéciales :
	{
		switch(pManagedDevice->DeviceStatus) // Selon l'état actuel de la Gestion du Device :
		{
		case I2cDeviceStatusLoaded:
			if(pManagedDevice->nbQueryRestart > I2CCM_ADD_IGNORE_QRY_RST_ON_PERIF_RST)
			{
				mayReInitPerif = 1; // Demande une Ré-Init de la HAL_I2C
			}
			break;	// Rien de plus à faire ici
		case I2cDeviceStatusLoadNext: // S'il faut charger un autre Device du même Groupe :
			pNewInitDevice = pManagedDevice->pInitDevice;   // A défaut : on rechargera l'actuel !

			// Préparer pour Effacer les Datas obsolètes :
			I2CCM_Device* pDevice = &(pManagedDevice->Device);

			mayReInitPerif = I2CCM_DEV_DEINIT_ALL_DATAS; // Par défaut, Réinitialiser toutes les variables !
			if(0 != pDevice->handleOnDeInitDevice)	// Commencer par invoquer le DeInit Handler :
			{
				mayReInitPerif = (0xFF & pDevice->handleOnDeInitDevice(pDevice)); // On effacera selon sa demande
			}

			if(0 != (I2CCM_DEV_DEINIT_INT_DATAS & mayReInitPerif))
			{
				I2CCM_InternalBaseData* pIntData = pDevice->pIntData;
				if(0 != pIntData) I2CCM_FillMemory((void*)pIntData, 0, pIntData->fullStructSize); // Effacer InternalDatas selon sa taille Totale
			}
			if(0 != (I2CCM_DEV_DEINIT_EXT_DATAS & mayReInitPerif))
			{
				I2CCM_ExternalBaseData* pExtData = pDevice->pExtData;
				if(0 != pExtData) I2CCM_FillMemory((void*)pExtData, 0, pExtData->fullStructSize); // Effacer ExternalDatas selon sa taille Totale
			}
			if(0 != (I2CCM_DEV_DEINIT_CORE_DATAS & mayReInitPerif))
			{
				I2CCM_FillMemory((void*)pDevice, 0, sizeof(I2CCM_Device));
			}

			// Prépare la recherche du remplaçant :
			int16_t newDeviceGroup = pNewInitDevice->deviceGroup;   // Récupère le Groupe à rechercher
			mayReInitPerif = 1; // Demande systématiquement une Ré-Init de la HAL_I2C

			void* startPtr = pNewInitDevice;
			// Recherche un autre Device du même Groupe :
			do
			{
				pNewInitDevice++;   // Pointe l'InitItem suivant
				I2CCM_REFRAME_PTR_WITHIN_OBJ_BASE_END(pNewInitDevice, pI2cDevManParams->pInitListBase, pI2cDevManParams->pInitListEnd);
				if(newDeviceGroup == pNewInitDevice->deviceGroup) // Si c'est le Groupe qu'on recherchait :
				{
					break; // Quitter directement pour effectuer le Chargement ...
				}
			} while(pNewInitDevice != startPtr);
			break;
		//--------------------------
		case I2cDeviceStatusNotInit:
		case I2cDeviceStatusDoReLoad:
		default:
			pNewInitDevice = pManagedDevice->pInitDevice;
			break;
		}

		// Cas où une ReInit de la HAL_I2C a été demandée :
		if(0 != mayReInitPerif)
		{
			uint8_t nbRequest2Restart = pManagedDevice->nbQueryRestart / I2CCM_ADD_IGNORE_QRY_RST_ON_PERIF_RST;
			if(nbRequest2Restart < pI2cDevManager->nbRestart) // On est dans la "marge de patience" :
			{
				mayReInitPerif = 0; // Abandonner le RéInit de la HAL_I2C, pour cette fois
			} else // On a atteint la "limite de patience" :
			{
				// Commencer par vérifier s'il y a pas quand même 1 raison d'éviter le ReInit de la HAL_I2C :
				for(I2CCM_Manage_Device* tmpDevice = pI2cDevManParams->pDevListBase; tmpDevice < pI2cDevManParams->pDevListEnd; tmpDevice++)
				{
					if(tmpDevice->nbComResultOK > I2CCM_NB_COM_RESULT_NO_ERR) mayReInitPerif = 0; // Il y a apparemment encore de la Com avec ce Device => on va éviter le ReInit de la HAL_I2C
					tmpDevice->nbComResultOK = I2CCM_NB_COM_RESULT_NO_ERR;	// Efface notre alibi
					tmpDevice->nbQueryRestart = 0;	// Efface toute autre éventuelle prochaine demande en attente
					tmpDevice->Device.ErrorsCt = 0; // RAZ des Compteurs d'erreur
				}
			}
		}
	}

	// Etape 2 : Traiter s'il y a 1 Device Handler à charger :
	if(IS_IN_RANGE(pNewInitDevice, pI2cDevManParams->pInitListBase, pI2cDevManParams->pInitListEnd -1)) // if(0 != pNewInitDevice)
	{
		// Prépare pour l'appel de l'Init :
		pManagedDevice->pInitDevice = pNewInitDevice;  // Mémorise l'InitItem qui sert au Chargement
		I2CCM_Device *pDevice = &pManagedDevice->Device;  // Pointe sur la Structure du Device

		// Si nécessaire : Penser à fournir ici le Buffer à pData si pas en Variable Globale !

		int16_t newDeviceResult = pNewInitDevice->pFnInit(pDevice, pNewInitDevice);  // Demande l'Init -> Retourne l'ID de ce SupportedDevice
		if(newDeviceResult > 0) // Si Succès de l'Init :
		{
			// Si nécessaire : Penser à fournir ici les Buffers de Tx & Rx si pas en Global !
			pManagedDevice->DeviceStatus = I2cDeviceStatusLoaded; // Pour Activer l'utilisation !
			pManagedDevice->nextActionTime = 0; // Start usage As soon as Possible
		} else { // Erreur de Chargement => Rechercher si autre choix :
			pManagedDevice->DeviceStatus = I2cDeviceStatusLoadNext;
		}
	}

	if(mayReInitPerif > 0)
	{
		if(pI2cDevManager->nbRestart < I2CCM_MAX_ALLOWED_RESTART_PERIF_CT) pI2cDevManager->nbRestart++; // Intègre le compteur de Restart directement dans le Core
	}
	return mayReInitPerif;
}

/******************************************************************************/

void i2cComMasterHandleActiveDevice(I2CCM_DevManager* pI2cDevManager, I2CCM_Void* hi2c)
{
	if(0 == pI2cDevManager) return;
	if(0 == hi2c) return;

	I2CCM_Manage_Device* pManagedDevice = pI2cDevManager->pManagedDevice;

	// Gérer le Device, s'il a bien été chargé :
	if(I2cDeviceStatusLoaded == pManagedDevice->DeviceStatus) // Si le Device est OK :
	{
		I2CCM_Device *pDevice = &pManagedDevice->Device;  // Pointe sur la Structure du Device

		// Gérer les Devices en mode Standard :
		uint16_t nextAction2Do = 0;
		do {
			// Vérifier s'il est temps de faire qqch ou pas pour ce Device :
			uint32_t curSysTick = I2C_SysTick();
			if(curSysTick < pManagedDevice->nextActionTime) break;
			if( ((curSysTick & INT32_MIN) > (pManagedDevice->nextActionTime & INT32_MIN)) && (pManagedDevice->nextActionTime > 0) ) break;	// Vérification compatibilité de l'éventuel rebouclage 32bits, sauf cas du reInit Device

			nextAction2Do = pDevice->getNextActionFrame(pDevice);

//            // Etape 1 : Y a-t-il des Demandes du Device ?
//            if( (0 != (I2C_NEED_PARAM & nextAction2Do)) &&
//                (0 != pDevice->getRwVarPtrSize) )
//            {
//
//            }

			// Etape 2 : Gérer les Envois à effectuer :
			if(0 != (I2C_TRANSMIT & nextAction2Do))
			{
				// ToDo : Ajouter ici le Switch vers le Channel I2C Required

				pDevice->result = I2C_Master_Transmit(hi2c, pDevice->DevAddr8,  pDevice->pTxBuf, pDevice->nbBytes2Send, pDevice->DelayInMs);

				// Informer de la fin de l'envoi (si demandé) :
				if(0 != (I2C_HANDLE_TX & nextAction2Do))
				{
					nextAction2Do = pDevice->handleActionComplete(pDevice);
				}

				// Traiter également à notre niveau le résultat de l'opération I2C_TX :
				i2cComMasterHandleComResult(&pManagedDevice->nbComResultOK, pDevice->result);
			}

			// Etape 3 : Gérer les Réceptions à demander :
			if(0 != (I2C_RECEIVE & nextAction2Do))
			{
				// ToDo : Ajouter ici le Switch vers le Channel I2C Required

				pDevice->result = I2C_Master_Receive(hi2c, pDevice->DevAddr8,  pDevice->pRxBuf, pDevice->nbBytes2Read, pDevice->DelayInMs);

				// Informer de la fin de la Réception (si demandé) :
				if(0 != (I2C_HANDLE_RX & nextAction2Do))
				{
					nextAction2Do = pDevice->handleActionComplete(pDevice);
				}

				// Traiter également à notre niveau le résultat de l'opération I2C_RX :
				i2cComMasterHandleComResult(&pManagedDevice->nbComResultOK, pDevice->result);
			}

//            // Etape 4 : Gérer les nouvelles valeurs disponibles :
//            if( (0 != (I2C_GET_VALUES & nextAction2Do)) &&
//                (0 != pDevice->getRwVarPtrSize) )
//            {
//
//            }

			// Etape 5 : Gérer les demandes de Reload / Abort :
			if(0 != (I2C_RELOAD_DEV & nextAction2Do))
			{
				pManagedDevice->DeviceStatus = I2cDeviceStatusDoReLoad;
			}
			if(0 != (I2C_ABORT_DEV & nextAction2Do))
			{
				pManagedDevice->DeviceStatus = I2cDeviceStatusLoadNext;
				if(pManagedDevice->nbQueryRestart < UINT8_MAX) { pManagedDevice->nbQueryRestart++; }
			}
			if(0 != (I2C_CHK_DEV_ERR & nextAction2Do))
			{
				if(SET == I2C_Master_IsI2cBusy(hi2c))	// Si le Bus est quand même resté occupé :
				{
					if(pManagedDevice->nbQueryRestart < UINT8_MAX) { pManagedDevice->nbQueryRestart++; }
				}
			}

			// Etape 6 : Gérer les demandes de Tempo :
			if(0 != (I2C_WAIT_TEMPO & nextAction2Do))
			{
				if(pDevice->DelayInMs > 0) I2C_Delay(pDevice->DelayInMs);
			}
			if(0 != (I2C_END_BLOC  & nextAction2Do))
			{
				pManagedDevice->nextActionTime = I2C_SysTick() + (pDevice->DelayInMs);
				if(0 != (I2C_NEW_VALUE & nextAction2Do)) // Lorsqu'on a pu récupérer nouvelle valeur => tout va bien, Communication (r)établie !
				{ // Remarque_Jp : remplacer par Notification Nouvelle data !!!
					pManagedDevice->nbQueryRestart = 0;
				}
				break;
			}
		} while(0 != ((I2C_TRANSMIT | I2C_RECEIVE | I2C_WAIT_TEMPO | I2C_NEED_PARAM | I2C_NEW_VALUE) & nextAction2Do));
	}

	// Préparer pour le prochain tour de Boucle :
	pI2cDevManager->pManagedDevice++; // Pointe le ManagedDevice suivant
}

/******************************************************************************/

void i2cComMasterHandleComResult(int8_t* pNbComResultOK, int32_t comResult)
{
	if(0 == comResult) // I2C Transmission/Reception OK :
	{
		if(*pNbComResultOK < I2CCM_NB_COM_RESULT_NO_ERR) *pNbComResultOK = I2CCM_NB_COM_RESULT_NO_ERR;
		if(*pNbComResultOK < I2CCM_MAX_NB_COM_RESULT_OK) (*pNbComResultOK)++;
	} else {
		if(*pNbComResultOK > I2CCM_MIN_NB_COM_RESULT_ERR) (*pNbComResultOK)--;
	}
}

/******************************************************************************/

void i2cComMasterAssertPin(I2CCM_AssertPinParams* pAssertPinParam, uint16_t newPinAssert)
{
	if(0 == pAssertPinParam) return;
	I2CCM_AssertDelayParams* pAssertDelay = &pAssertPinParam->AssertDelay[I2CCM_DO_ASSERT_PIN == newPinAssert];

	if(pAssertDelay->msBefore > 0) I2C_Delay(pAssertDelay->msBefore);

	if(0 != pAssertPinParam->GPIOx)
	{
		I2CCM_SET_PIN_STATE(pAssertPinParam->GPIOx, pAssertPinParam->GPIO_Pin,
				pAssertPinParam->ActivePinState ^ (I2CCM_DO_ASSERT_PIN == newPinAssert ? GPIO_PIN_RESET : GPIO_PIN_SET));
	}

	if(pAssertDelay->msAfter > 0) I2C_Delay(pAssertDelay->msAfter);
}

/******************************************************************************/

I2CCM_Manage_Device* i2cComMasterGetDeviceManagerByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar)
{
	if(0 == pI2cDevManParams) return 0;
	if(0 == pExtVar) return 0;

	for(I2CCM_Manage_Device* tmpDevice = pI2cDevManParams->pDevListBase; tmpDevice < pI2cDevManParams->pDevListEnd; tmpDevice++)
	{
		if(I2cDeviceStatusLoaded == tmpDevice->DeviceStatus) // Si le Device est bien chargé :
		{
			if(pExtVar == tmpDevice->Device.pExtData) return tmpDevice;	// OK, le Pointeur correspond
		}
	}

	return 0;	// Failure : External Var Pointer Not found in the List !
}

/******************************************************************************/

uint8_t i2cComMasterHandleExternalActionByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar, I2CCM_ExternalAction* pExtAction)
{
	if(0 == pI2cDevManParams) return 0;
	if(0 == pExtVar) return 0;

	I2CCM_MAKE_TYPED_PTR(I2CCM_ExternalBaseData, pExtData, pExtVar); // La Base de la Structure pointée par pExtVar est forcément de type "I2CCM_ExternalBaseData" !
	if( (0 != pExtData->idOfDevice) && (0 == (pExtData->idOfDevice & I2CCM_DEVICE_RECENTLY_LOADED_MSK)) ) // Si Référencement & Initialisation terminés !
	{
		I2CCM_Manage_Device* pManagedDevice = i2cComMasterGetDeviceManagerByExtVarPtr(pI2cDevManParams, pExtVar);
		if(0 != pManagedDevice)
		{
			I2CCM_Device *pDevice = &pManagedDevice->Device;  // Pointe sur la Structure du Device
			pDevice->pUserParam = pExtAction;	// Transfère l'Argument fourni au Device

			// Gérer le Device en mode Spécial :
			if(0 != pDevice->handleExternalAction)
			{
				uint16_t nextAction2Do = 0;
				do {
					nextAction2Do = pDevice->handleExternalAction(pDevice);

					// Etape 5 : Gérer les demandes de Reload / Abort :
					if(0 != (I2C_RELOAD_DEV & nextAction2Do))
					{
						pManagedDevice->DeviceStatus = I2cDeviceStatusDoReLoad;
					}
					if(0 != (I2C_ABORT_DEV & nextAction2Do))
					{
						pManagedDevice->DeviceStatus = I2cDeviceStatusLoadNext;
						if(pManagedDevice->nbQueryRestart < UINT8_MAX) { pManagedDevice->nbQueryRestart++; }
					}

					// Etape 6 : Gérer les demandes de Tempo :
					if(0 != (I2C_WAIT_TEMPO & nextAction2Do))
					{
						if(pDevice->DelayInMs > 0) I2C_Delay(pDevice->DelayInMs);
					}
					if(0 != (I2C_END_BLOC  & nextAction2Do))
					{
						pManagedDevice->nextActionTime = I2C_SysTick() + (pDevice->DelayInMs);
						if(0 != (I2C_NEW_VALUE & nextAction2Do)) // Lorsqu'on a pu récupérer nouvelle valeur => tout va bien, Communication (r)établie !
						{ // Remarque_Jp : remplacer par Notification Nouvelle data !!!
							pManagedDevice->nbQueryRestart = 0;
						}
						break;
					}

				} while(0 != ((I2C_TRANSMIT | I2C_RECEIVE | I2C_WAIT_TEMPO | I2C_NEED_PARAM | I2C_NEW_VALUE) & nextAction2Do));
			}
			return 1;
		}
	}

	return 0;	// Failure : External Var Pointer Not found in the List !
}

/******************************************************************************/

uint8_t i2cComMasterForceDeviceNextActionDelayByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar, uint32_t DelayInMs)
{
	if(0 == pI2cDevManParams) return 0;
	if(0 == pExtVar) return 0;

	I2CCM_MAKE_TYPED_PTR(I2CCM_ExternalBaseData, pExtData, pExtVar); // La Base de la Structure pointée par pExtVar est forcément de type "I2CCM_ExternalBaseData" !
//	I2CCM_ExternalBaseData* pExtData = (I2CCM_ExternalBaseData*)pExtVar; // La Base de la Structure pointée par pExtVar est forcément de type "I2CCM_ExternalBaseData" !
	if( (0 != pExtData->idOfDevice) && (0 == (pExtData->idOfDevice & I2CCM_DEVICE_RECENTLY_LOADED_MSK)) ) // Si Référencement & Initialisation terminés !
	{
		I2CCM_Manage_Device* tmpDevice = i2cComMasterGetDeviceManagerByExtVarPtr(pI2cDevManParams, pExtVar);
		if(0 != tmpDevice)
		{
			tmpDevice->nextActionTime = I2C_SysTick() + DelayInMs;
			return 1;
		}
//		for(I2CCM_Manage_Device *tmpDevice = pI2cDevManParams->pDevListBase; tmpDevice < pI2cDevManParams->pDevListEnd; tmpDevice++)
//		{
//			if(I2cDeviceStatusLoaded == tmpDevice->DeviceStatus) // Si le Device est bien chargé :
//			{
//				I2CCM_Device *pDevice = &(tmpDevice->Device);  // Pointe sur la Structure du Device
//				if(pDevice->pExtData == pExtVar)
//				{
//					tmpDevice->nextActionTime = I2C_SysTick() + DelayInMs;
//					return 1;
//				}
//			}
//		}
	}

	return 0;	// Failure : External Var Pointer Not found in the List !
}

/******************************************************************************/

uint16_t i2cComMaster_FinalizeSizedRequest(I2CCM_Device *pDevice, uint8_t nbBytes2Send, uint8_t nbBytes2Read)
{
	pDevice->nbBytes2Send = nbBytes2Send;
	pDevice->nbBytes2Read = nbBytes2Read;
	return ((nbBytes2Send > 0) ? I2C_TRANSMIT : 0) | ((nbBytes2Read > 0) ? I2C_RECEIVE | I2C_HANDLE_RX : I2C_HANDLE_TX);
}

/******************************************************************************/

uint16_t i2cComMaster_BuildRequest_Cmd16BEwithReply(I2CCM_Device *pDevice, uint16_t Cmd16, uint8_t nbBytes2Read)
{
	pDevice->nbBytes2Send = i2cComMaster_PushCmd16BigEndian(pDevice->pTxBuf, Cmd16);
	pDevice->nbBytes2Read = nbBytes2Read;
	return I2C_TRANSMIT | ((nbBytes2Read > 0) ? I2C_RECEIVE | I2C_HANDLE_RX : I2C_HANDLE_TX);
}

/******************************************************************************/

uint16_t i2cComMaster_BuildRequest_Cmd16BEwithArg16BE(I2CCM_Device *pDevice, uint16_t Cmd16, uint16_t Arg16)
{
	pDevice->nbBytes2Send = i2cComMaster_PushCmd16BE_Arg16BE(pDevice->pTxBuf, Cmd16, Arg16);
	return I2C_TRANSMIT | I2C_HANDLE_TX; // Demande Envoi + CallBack de Réception
}

/******************************************************************************/

uint16_t i2cComMaster_BuildRequest_Cmd16BE_Arg16BE_CRC8(I2CCM_Device *pDevice, uint16_t Cmd16, uint16_t Arg16, uint8_t Crc8)
{
	pDevice->nbBytes2Send = i2cComMaster_PushCmd16BE_Arg16BE_CRC8(pDevice->pTxBuf, Cmd16, Arg16, Crc8);
	return I2C_TRANSMIT | I2C_HANDLE_TX; // Demande Envoi + CallBack de Réception
}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd8_Arg16BigEndian(uint8_t* pBuf, uint8_t Cmd8, uint16_t Arg16)
{
	pBuf[0] = Cmd8;
	pBuf[1] = I2CCM_GET_BYTE_N(Arg16, 1);	// BigEndian => MSB first
	pBuf[2] = I2CCM_GET_BYTE_N(Arg16, 0);	//  then LSB in last
	return sizeof(Cmd8) + sizeof(Arg16);
}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd8_Arg16LittleEndian(uint8_t* pBuf, uint8_t Cmd8, uint16_t Arg16)
{
	pBuf[0] = Cmd8;
	pBuf[1] = I2CCM_GET_BYTE_N(Arg16, 0);	// LittleEndian => LSB first
	pBuf[2] = I2CCM_GET_BYTE_N(Arg16, 1);	//  then MSB in last
	return sizeof(Cmd8) + sizeof(Arg16);
}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd8_Arg16_16BigEndian(uint8_t* pBuf, uint8_t Cmd8, uint16_t Arg16A, uint16_t Arg16B)
{
	pBuf[0] = Cmd8;
	pBuf[1] = I2CCM_GET_BYTE_N(Arg16A, 1);	// BigEndian => MSB first
	pBuf[2] = I2CCM_GET_BYTE_N(Arg16A, 0);	//  then LSB in last
	pBuf[3] = I2CCM_GET_BYTE_N(Arg16B, 1);	// BigEndian => MSB first
	pBuf[4] = I2CCM_GET_BYTE_N(Arg16B, 0);	//  then LSB in last
	return sizeof(Cmd8) + sizeof(Arg16A) + sizeof(Arg16B);
}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd16BigEndian(uint8_t* pBuf, uint16_t Cmd16)
{
	pBuf[0] = I2CCM_GET_BYTE_N(Cmd16, 1);	// BigEndian => MSB first
	pBuf[1] = I2CCM_GET_BYTE_N(Cmd16, 0);	// then LSB in last
	return sizeof(Cmd16);
}

/******************************************************************************/

//uint8_t i2cComMaster_PushCmd16BE_Arg16BE(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16)
//{
//	pBuf[0] = I2CCM_GET_BYTE_N(Cmd16, 1);	// BigEndian => Command MSB first
//	pBuf[1] = I2CCM_GET_BYTE_N(Cmd16, 0);	//  then Command LSB
//	pBuf[2] = I2CCM_GET_BYTE_N(Arg16, 1);	// BigEndian => Arg16 MSB first
//	pBuf[3] = I2CCM_GET_BYTE_N(Arg16, 0);	//  and Arg16 LSB in last
//	return sizeof(Cmd16) + sizeof(Arg16);
//}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd16BE_Arg16BE_CRC8(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16, uint8_t Crc8)
{
	pBuf[0] = I2CCM_GET_BYTE_N(Cmd16, 1);	// BigEndian => Command MSB first
	pBuf[1] = I2CCM_GET_BYTE_N(Cmd16, 0);	//  then Command LSB
	pBuf[2] = I2CCM_GET_BYTE_N(Arg16, 1);	// BigEndian => Arg16 MSB first
	pBuf[3] = I2CCM_GET_BYTE_N(Arg16, 0);	//  then Arg16 LSB part
	pBuf[4] = Crc8;
	return sizeof(Cmd16) + sizeof(Arg16) + sizeof(Crc8);
}

/******************************************************************************/

uint8_t i2cComMaster_PushCmd16BE_Arg16_16BE_CRC8(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16A, uint8_t Crc8A, uint16_t Arg16B, uint8_t Crc8B)
{
	pBuf[0] = I2CCM_GET_BYTE_N(Cmd16, 1);	// BigEndian => Command MSB first
	pBuf[1] = I2CCM_GET_BYTE_N(Cmd16, 0);	//  then Command LSB
	pBuf[2] = I2CCM_GET_BYTE_N(Arg16A, 1);	// BigEndian => Arg16 MSB first
	pBuf[3] = I2CCM_GET_BYTE_N(Arg16A, 0);	//  then Arg16 LSB part
	pBuf[4] = Crc8A;
	pBuf[5] = I2CCM_GET_BYTE_N(Arg16B, 1);	// BigEndian => Arg16 MSB first
	pBuf[6] = I2CCM_GET_BYTE_N(Arg16B, 0);	//  then Arg16 LSB part
	pBuf[7] = Crc8B;
	return sizeof(Cmd16) + sizeof(Arg16A) + sizeof(Crc8A) + sizeof(Arg16B) + sizeof(Crc8B);
}

/******************************************************************************/

float i2cComMaster_FloatFromRaw(uint32_t floatRaw)
{
	union {
		uint32_t raw;
		float floatValue;
	} ufloatRaw;
	ufloatRaw.raw = floatRaw;
	return ufloatRaw.floatValue;
}

/******************************************************************************/

float i2cComMaster_AddInt16ToMoyManager(I2CCM_MoyManager_Int16* pMoyManager, int16_t newValue, uint16_t maxCt)
{
	I2CCM_REFRAME_TO_MAX_OF_RANGE(pMoyManager->maxCt, 1, maxCt);
	if(pMoyManager->maxCt < 1) return 0.0f;
	int16_t* pTabData = (void*)pMoyManager + sizeof(I2CCM_MoyManager_Int16);	// Pointe le Tableau de Data qui suit notre Structure
	uint16_t curIndex = pMoyManager->curIdx;
	if(curIndex >= pMoyManager->maxCt) { curIndex = 0; }	// reboucle en cas de dépassement de l'index
	pMoyManager->sumTab -= pTabData[curIndex];
	pMoyManager->sumTab += pTabData[curIndex] = newValue;
	pMoyManager->curIdx = curIndex +1;	// Décale vers l'emplacement suivant
	if(++pMoyManager->nbInTab > pMoyManager->maxCt) { pMoyManager->nbInTab = pMoyManager->maxCt; } // Plafonne le nb d'éléments au Max
	return ((float)(pMoyManager->sumTab) / (float)(pMoyManager->nbInTab));
}

/******************************************************************************/

float i2cComMaster_AddUInt16ToMoyManager(I2CCM_MoyManager_UInt16* pMoyManager, uint16_t newValue, uint16_t maxCt)
{
	I2CCM_REFRAME_TO_MAX_OF_RANGE(pMoyManager->maxCt, 1, maxCt);
	if(pMoyManager->maxCt < 1) return 0.0f;
	uint16_t* pTabData = (void*)pMoyManager + sizeof(I2CCM_MoyManager_UInt16);	// Pointe le Tableau de Data qui suit notre Structure
	uint16_t curIndex = pMoyManager->curIdx;
	if(curIndex >= pMoyManager->maxCt) { curIndex = 0; }	// reboucle en cas de dépassement de l'index
	pMoyManager->sumTab -= pTabData[curIndex];
	pMoyManager->sumTab += pTabData[curIndex] = newValue;
	pMoyManager->curIdx = curIndex +1;	// Décale vers l'emplacement suivant
	if(++pMoyManager->nbInTab > pMoyManager->maxCt) { pMoyManager->nbInTab = pMoyManager->maxCt; } // Plafonne le nb d'éléments au Max
	return ((float)(pMoyManager->sumTab) / (float)(pMoyManager->nbInTab));
}

/******************************************************************************/

float i2cComMaster_AddFloatToMoyManager(I2CCM_MoyManager_Float* pMoyManager, float newValue, uint16_t maxCt)
{
	I2CCM_REFRAME_TO_MAX_OF_RANGE(pMoyManager->maxCt, 1, maxCt);
	if(pMoyManager->maxCt < 1) return 0.0f;
	float* pTabData = (void*)pMoyManager + sizeof(I2CCM_MoyManager_Float);	// Pointe le Tableau de Data qui suit notre Structure
	uint16_t curIndex = pMoyManager->curIdx;
	if(curIndex >= pMoyManager->maxCt) { curIndex = 0; }	// reboucle en cas de dépassement de l'index
	pMoyManager->sumTab -= pTabData[curIndex];
	pMoyManager->sumTab += pTabData[curIndex] = newValue;
	pMoyManager->curIdx = curIndex +1;	// Décale vers l'emplacement suivant
	if(++pMoyManager->nbInTab > pMoyManager->maxCt) { pMoyManager->nbInTab = pMoyManager->maxCt; } // Plafonne le nb d'éléments au Max
	return ((float)(pMoyManager->sumTab) / (float)(pMoyManager->nbInTab));
}

/******************************************************************************/

#ifndef UTILS_UTILS_H_	// en cas d'absence de "utils.h" :
	inline void I2CCM_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt) { while(BytesCt-- > 0) { *pDest++ = Value; } }
	inline void I2CCM_CopyMemory(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt) { while(BytesCt-- > 0) { *pDest++ = *pSrc++; } }
#endif // UTILS_UTILS_H_
