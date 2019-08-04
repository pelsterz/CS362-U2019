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
int checkTributeCard(int p, int next_p, struct gameState *post){
  struct gameState pre;
  int revealed[2] = {-1, -1};
  memcpy (&pre, post, sizeof(struct gameState));

  int r;

  int choice[] = {0};
  r = _tribute_effect(choice, 0, post);

  int j, i, c, count, deckCounter;
  if ((pre.discardCount[next_p] + pre.deckCount[next_p]) <= 1){
    if (pre.deckCount[next_p] > 0){
      revealed[0] = pre.deck[next_p][pre.deckCount[next_p]-1];
      pre.deckCount[next_p]--;
    }
    else if (pre.discardCount[next_p] > 0){
      revealed[0] = pre.discard[next_p][pre.discardCount[next_p]-1];
      pre.discardCount[next_p]--;
    }
  }

  else{
    if (pre.deckCount[next_p] == 0){
      for (i = 0; i < pre.discardCount[next_p]; i++){
        pre.deck[next_p][i] = pre.discard[next_p][i];
        pre.deckCount[next_p]++;
        pre.discard[next_p][i] = -1;
        pre.discardCount[next_p]--;
      }
      shuffle(next_p, &pre);
    }
    revealed[0] = pre.deck[next_p][pre.deckCount[next_p]-1];
    pre.deck[next_p][pre.deckCount[next_p]--] = -1;
    pre.deckCount[next_p]--;
    revealed[1] = pre.deck[next_p][pre.deckCount[next_p]-1];
    pre.deck[next_p][pre.deckCount[next_p]--] = -1;
    pre.deckCount[next_p]--;
  }

  if (revealed[0] == revealed[1]){
    pre.tabledCards[pre.tabledCardCount] = revealed[1];
    pre.tabledCardCount++;
    revealed[1] = -1;
  }

  for (j = 0; j <= 2; j++){
    if (revealed[j] == copper || revealed[j] == silver || revealed[j] == gold){
      pre.coins += 2;
    }
    else if (revealed[j] == estate || revealed[j] == duchy || revealed[j] == province || revealed[j] == gardens || revealed[j] == great_hall){
      // Draw 2 cards
      for(c = 0; c < 2; c++){
        // Draw cards (copied from drawCard)
        if (pre.deckCount[p] <= 0){
          for (i = 0; i < pre.discardCount[p];i++){
            pre.deck[p][i] = pre.discard[p][i];
            pre.discard[p][i] = -1;
          }

          pre.deckCount[p] = pre.discardCount[p];
          pre.discardCount[p] = 0;

          shuffle(p, &pre);

          pre.discardCount[p] = 0;
          count = pre.handCount[p];

          deckCounter = pre.deckCount[p];

          if (deckCounter == 0) return -1;

          pre.hand[p][count] = pre.deck[p][deckCounter - 1];
          pre.deckCount[p]--;
          pre.handCount[p]++;
        }
        else{
          count = pre.handCount[p];
          deckCounter = pre.deckCount[p];
          pre.hand[p][count] = pre.deck[p][deckCounter - 1];
          pre.deckCount[p]--;
          pre.handCount[p]++;
        }
      }
    }
    else if (revealed[j] != curse && revealed[j] != -1){
      pre.numActions = pre.numActions + 2;
    }
  }

  if (assertIntEqual(r, 0) && assertStateEqual(&pre, post)) return 1;
  else return 0;
}

int main() {
  int n, i, numTests = 10, count = 0;
  clock_t t;
  struct gameState G;
  int maxp = 4;
  int p, next_p;

  // Run random tests
  printf ("TESTING tributeCard():\n");
  t = clock();
  for (n = 0; n < numTests; n++){
    for(i = 0; i < sizeof(struct gameState); i++){
      ((char*)&G)[i] = floor(Random() *256);
    }
    p = floor(Random() * maxp);
    G.whoseTurn = p;
    if(p == 3) next_p = 0;
    else next_p = p + 1;
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);
    G.tabledCardCount = floor(Random() * MAX_DECK);
    G.handCount[p] = floor(Random() * MAX_HAND);
    for(i = 0; i < G.handCount[p]; i++){
      G.hand[p][i] = floor(Random() * 27);
    }
    if(n < numTests/4){
      // Narrower random selection to allow higher chance of values <= 1
      G.deckCount[next_p] = floor(Random() * 20);
      G.discardCount[next_p] = floor(Random() * 20);
    } else {
      // Check full range of possible values for other tests
      G.deckCount[next_p] = floor(Random() * MAX_DECK);
      G.discardCount[next_p] = floor(Random() * MAX_DECK);
    }
    G.handCount[next_p] = floor(Random() * MAX_HAND);
    for(i = 0; i < G.deckCount[next_p]; i++){
      G.deck[next_p][i] = floor(Random() * 27);
    }
    for(i = 0; i < G.tabledCardCount; i++){
      G.tabledCards[i] = floor(Random() * 27);
    }
    count += checkTributeCard(p, next_p, &G);
  }

  t = clock() - t;
  printf("%d out of %d passed, taking %f seconds\n", count, numTests, ((float)t) / CLOCKS_PER_SEC);

  return 0;
}
