#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

// set NOISY_TEST to 0 to remove printfs describing tests from output
#define NOISY_TEST 0

// Custom integer assert function (returns 1 if true)
// a = Actual, b = Expected
int assertIntEqual(int a, int b){
  #if (NOISY_TEST == 1)
    printf("Actual: %d, Expected: %d\n", a, b);
  #endif
  if(a == b) return 1;
  else return 0;
}

// Custom gameState assert function (returns 1 if true)
// a = Pre, b = Post
int assertStateEqual(struct gameState *pre, struct gameState *post){
  if(memcmp(pre, post, sizeof(struct gameState)) == 0) return 1;
  else return 0;
}

// Compares before and after of gameState for card
// return 1 = success, return 0 = failure
int checkMinionCard(int choice1, int choice2, int handPos, int p, struct gameState *post){
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;

  int choice[] = {choice1, choice2};
  r = _minion_effect(choice, 2, post);

  int player, c, i, count, deckCounter;
  pre.numActions++;

  // Discard card
  discardCard(p, &pre, handPos);

  if(choice1) pre.coins += 2;
  else if(choice2){
    // Check all players to possible dicard hand and draw cards
    for(player = 0; player < pre.numPlayers; player++){
      if(pre.handCount[player] > 4 || player == p){
        // Discard hand
        while(pre.handCount[player] > 0){
          // Discard card (copied from discardCard)
          pre.tabledCards[pre.tabledCardCount] = pre.hand[player][handPos];
          pre.tabledCardCount++;

          pre.hand[player][handPos] = -1;

          if ( handPos == (pre.handCount[player] - 1) ) {
            pre.handCount[player]--;
          }
          else if ( pre.handCount[player] == 1 ) {
            pre.handCount[player]--;
          }
          else {
            pre.hand[player][handPos] = pre.hand[player][ (pre.handCount[player] - 1)];
            pre.hand[player][pre.handCount[player] - 1] = -1;
            pre.handCount[player]--;
          }
        }
        // Draw 4 cards
        for(c = 0; c < 4; c++){
          // Draw cards (copied from drawCard)
          if (pre.deckCount[player] <= 0){
            for (i = 0; i < pre.discardCount[player];i++){
              pre.deck[player][i] = pre.discard[player][i];
              pre.discard[player][i] = -1;
            }

            pre.deckCount[player] = pre.discardCount[player];
            pre.discardCount[player] = 0;

            shuffle(player, &pre);

            pre.discardCount[player] = 0;
            count = pre.handCount[player];
            deckCounter = pre.deckCount[player];
            if (deckCounter == 0) return -1;
            pre.hand[player][count] = pre.deck[player][deckCounter - 1];
            pre.deckCount[player]--;
            pre.handCount[player]++;
          }
          else{
            count = pre.handCount[player];
            deckCounter = pre.deckCount[player];
            pre.hand[player][count] = pre.deck[player][deckCounter - 1];
            pre.deckCount[player]--;
            pre.handCount[player]++;
          }
        }
      }
    }
  }

  if (assertIntEqual(r, 0) && assertStateEqual(&pre, post)) return 1;
  else return 0;
}

int main() {
  int n, i, numTests = 20, count = 0;
  clock_t t;
  struct gameState G;
  int maxp = 4;
  int player, p, choice1, choice2, handPos;

  // Run random tests
  printf ("TESTING minionCard():\n");
  t = clock();
  for (n = 0; n < numTests; n++){
    for(i = 0; i < sizeof(struct gameState); i++){
      ((char*)&G)[i] = floor(Random() *256);
    }
    G.numPlayers = floor(Random() * maxp + 1);
    for(player = 0; player < G.numPlayers; player++){
      G.deckCount[player] = floor(Random() * MAX_DECK);
      G.discardCount[player] = floor(Random() * MAX_DECK);
      G.handCount[player] = floor(Random() * MAX_HAND);
      for(i = 0; i < G.handCount[player]; i++){
        G.hand[player][i] = floor(Random() * 27);
      }
      for(i = 0; i < G.deckCount[player]; i++){
        G.deck[player][i] = floor(Random() * 27);
      }
    }
    G.tabledCardCount = floor(Random() * MAX_DECK);
    p = floor(Random() * G.numPlayers);
    G.whoseTurn = p;
    handPos = floor(Random() * G.handCount[p]);
    choice1 = floor(Random() * 2);
    choice2 = floor(Random() * 2);
    for(i = 0; i < G.tabledCardCount; i++){
      G.tabledCards[i] = floor(Random() * 27);
    }
    G.supplyCount[estate] = floor(Random() * 16);
    count += checkMinionCard(choice1, choice2, handPos, p, &G);
  }

  t = clock() - t;
  printf("%d out of %d passed, taking %f seconds\n", count, numTests, ((float)t) / CLOCKS_PER_SEC);

  return 0;
}
