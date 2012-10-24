/*******************************************************************************
Copyright (C) Marvell Interfdbional Ltd. and its affiliates

This software file (the "File") is owned and distributed by Marvell
Interfdbional Ltd. and/or its affiliates ("Marvell") under the following
alterfdbive licensing terms.  Once you have made an election to distribute the
File under one of the following license alterfdbives, please (i) delete this
introductory statement regarding license alterfdbives, (ii) delete the two
license alterfdbives that you have not elected to use and (iii) preserve the
Marvell copyright notice above.

********************************************************************************
Marvell Commercial License Option

If you received this File from Marvell and you have entered into a commercial
license agreement (a "Commercial License") with Marvell, the File is licensed
to you under the terms of the applicable Commercial License.

********************************************************************************
Marvell GPL License Option

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File in accordance with the terms and conditions of the General
Public License Version 2, June 1991 (the "GPL License"), a copy of which is
available along with the File in the license.txt file or by writing to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
on the worldwide web at http://www.gnu.org/licenses/gpl.txt.

THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
DISCLAIMED.  The GPL License provides additional details about this warranty
disclaimer.
********************************************************************************
Marvell BSD License Option

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File under the following licensing terms.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    *   Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

    *   Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.

    *   Neither the name of Marvell nor the names of its contributors may be
	used to endorse or promote products derived from this software without
	specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/

/*******************************************************************************
* mvNfpFib.c - Marvell Fast Network Processing
*
* DESCRIPTION:
*
*       Supported Features:
*       - OS independent.
*
*******************************************************************************/

#include "mvOs.h"
#include "mvDebug.h"
#include "gbe/mvNeta.h"
#include "mvNfpDefs.h"
#include "mvNfp.h"

/*#define NFP_DBG(x...) mvOsPrintf(x)*/
#define NFP_DBG(x...)

NFP_RULE_BRIDGE **nfp_bridge_hash = NULL;

MV_STATUS _INIT mvNfpBridgeInit(void)
{
	MV_U32 bytes = sizeof(NFP_RULE_BRIDGE *) * NFP_BRIDGE_HASH_SIZE;

	nfp_bridge_hash = (NFP_RULE_BRIDGE **)mvOsMalloc(bytes);
	if (nfp_bridge_hash == NULL) {
		mvOsPrintf("NFP (bridge): not enough memory\n");
		return MV_NO_RESOURCE;
	}

	mvOsMemset(nfp_bridge_hash, 0, bytes);

	mvOsPrintf("NFP (bridge) init %d entries, %d bytes\n", NFP_BRIDGE_HASH_SIZE, bytes);

	return MV_OK;
}

/* Clear Bridge Rule Database */
MV_STATUS mvNfpBridgeClear(void)
{
	int	i;
	NFP_RULE_BRIDGE	*rule, *tmp;

	if (nfp_bridge_hash == NULL)
		return MV_NOT_INITIALIZED;

	for (i = 0; i < NFP_BRIDGE_HASH_SIZE; i++) {

		rule = nfp_bridge_hash[i];
		while (rule) {
			tmp = rule;
			rule = rule->next;
			mvOsFree(tmp);
		}
		nfp_bridge_hash[i] = NULL;
	}
	return MV_OK;
}

void mvNfpBridgeDestroy(void)
{
	if (nfp_bridge_hash != NULL)
		mvOsFree(nfp_bridge_hash);
}


MV_STATUS mvNfpBridgeRuleAdd(NFP_RULE_BRIDGE *rule2)
{
	MV_U32 hash;
	NFP_RULE_BRIDGE *rule;

	hash = mvNfpBridgeRuleHash(rule2->bridgeId, rule2->mac);
	hash &= NFP_BRIDGE_HASH_MASK;
	rule = nfp_bridge_hash[hash];

	while (rule) {
		if (mvNfpBridgeRuleCmp(rule2->bridgeId, rule2->mac, rule)) {
			MV_U32 age = rule->age;

			/* Update rule, but save age */
			mvOsMemcpy(rule, rule2, sizeof(NFP_RULE_BRIDGE));
			rule->age = age;
			goto out;
		}
		rule = rule->next;
	}

	rule = (NFP_RULE_BRIDGE *)mvOsMalloc(sizeof(NFP_RULE_BRIDGE));
	if (!rule) {
		mvOsPrintf("NFP (bridge) %s OOM\n", __func__);
		return MV_FAIL;
	}

	mvOsMemcpy(rule, rule2, sizeof(NFP_RULE_BRIDGE));

	rule->next = nfp_bridge_hash[hash];
	nfp_bridge_hash[hash] = rule;

	/* lookup incomplete FIB entries */

out:
	NFP_DBG("NFP (bridge) add %p\n", rule);

	return MV_OK;
}

MV_STATUS mvNfpBridgeRuleDel(NFP_RULE_BRIDGE *rule2)
{
	MV_U32 hash;
	NFP_RULE_BRIDGE *rule, *prev;

	hash = 0;
	hash &= NFP_BRIDGE_HASH_MASK;

	rule = nfp_bridge_hash[hash];
	prev = NULL;

	while (rule) {
		if (mvNfpBridgeRuleCmp(rule2->bridgeId, rule2->mac, rule))
			return MV_OK;

		prev = rule;
		rule = rule->next;
	}
	return MV_NOT_FOUND;
}

MV_U32 mvNfpBridgeRuleAge(NFP_RULE_BRIDGE *rule2)
{
	MV_U32 hash, age;
	NFP_RULE_BRIDGE *rule;

	hash = mvNfpBridgeRuleHash(rule2->bridgeId, rule2->mac);
	hash &= NFP_BRIDGE_HASH_MASK;

	rule = nfp_bridge_hash[hash];
	while (rule) {

		if (mvNfpBridgeRuleCmp(rule2->bridgeId, rule2->mac, rule)) {
			age = rule->age;
			rule->age = 0;
			return age;
		}
		rule = rule->next;
	}
	return 0;
}

static void mvNfpBridgeRulePrint(NFP_RULE_BRIDGE *rule)
{
	if (rule->status == NFP_BRIDGE_INV)
		mvOsPrintf("INVALID  : ");
	else if (rule->status == NFP_BRIDGE_LOCAL)
		mvOsPrintf("LOCAL    : ");
	else
		mvOsPrintf("NON_LOCAL: ");

	mvOsPrintf(" bridgeId=%d mac=" MV_MACQUAD_FMT " mh_out=%02x:%02x outport=%d, outdev=%p, age=%u\n",
				rule->bridgeId, MV_MACQUAD(rule->mac), ((MV_U8 *)&rule->mh_out)[0], ((MV_U8 *)&rule->mh_out)[1],
				rule->outport, rule->outdev, rule->age);
}


void mvNfpBridgeDump(void)
{
	MV_U32 i;
	NFP_RULE_BRIDGE *rule;

	mvOsPrintf("(bridge)\n");
	for (i = 0; i < NFP_BRIDGE_HASH_SIZE; i++) {
		rule = nfp_bridge_hash[i];

		while (rule) {
			mvOsPrintf(" [%5d] ", i);
			mvNfpBridgeRulePrint(rule);
			rule = rule->next;
		}
	}
}
