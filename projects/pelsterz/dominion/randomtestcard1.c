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
int checkBaronCard(int choice1, int p, struct gameState *post){
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;

  r = baronCard(0, choice1, 0, 0, post, 0, 0, p);

  int i = 0;
  pre.numBuys++;
  while(choice1 > 0 && i < pre.handCount[p]){
    if(pre.hand[p][i] == estate){
      pre.coins += 4;//Add 4 coins to the amount of coins
      pre.discard[p][pre.discardCount[p]] = pre.hand[p][i];
      pre.discardCount[p]++;
      for (;i < pre.handCount[p]; i++){
        pre.hand[p][i] = pre.hand[p][i+1];
      }
      pre.hand[p][pre.handCount[p]] = -1;
      pre.handCount[p]--;
    }
    i++;
  }
  if(choice1 <= 0 || (i < pre.handCount[p] && pre.supplyCount[estate] > 0)){
    if(pre.supplyCount[estate] > 0)
      pre.discardCount[p]++;
      pre.supplyCount[estate]--;
  }

  if (assertIntEqual(r, 0) && assertStateEqual(&pre, post)) return 1;
  else return 0;
}

int main() {
  int n, i, numTests = 2000, count = 0;
  clock_t t;
  struct gameState G;
  int maxPlayer = 4;
  int p, choice1;

  // Run random tests
  printf ("TESTING baronCard():\n");
  t = clock();
  for (n = 0; n < numTests; n++){
    for(i = 0; i < sizeof(struct gameState); i++){
      ((char*)&G)[i] = floor(Random() *256);
    }
    p = floor(Random() * maxPlayer);
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);
    G.handCount[p] = floor(Random() * MAX_HAND);
    choice1 = floor(Random() * 2);
    for(i = 0; i < G.handCount[p]; i++){
      G.hand[p][i] = floor(Random() * 27);
    }
    G.supplyCount[estate] = floor(Random() * 16);
    count += checkBaronCard(choice1, p, &G);
  }

  t = clock() - t;
  printf("%d out of %d passed, taking %f seconds\n", count, numTests, ((float)t) / CLOCKS_PER_SEC);

  return 0;
}
