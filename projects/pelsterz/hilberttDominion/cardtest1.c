#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// set NOISY_TEST to 0 to remove printfs describing tests from output
#define NOISY_TEST 0

// Custom assert function (returns 1 if true, allows number of successes to be counted)
// a = Actual, b = Expected
int asserttrue(int a, int b){
  printf("Actual: %d, Expected: %d\n", a, b);
  if(a == b) return 1;
  else return 0;
}

int main() {
  int i, temp = 0, total = 0, count = 0;
  int seed = 1000;
  int numPlayer = 2;
  int maxBonus = 10;
  int p, r, c, h = 0, handCount;
  int bonus = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel,
    smithy, village, baron, great_hall};
  struct gameState G;
  int maxHandCount = 5;

  printf ("TESTING initializeGame():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state

      // Check number of players is correct
      #if (NOISY_TEST == 1)
        printf("Check number of players is correct\n");
      #endif
      r = initializeGame(numPlayer, k, seed, &G);
      count += asserttrue(G.numPlayers, 2); // check if the number of players is correct
      total++;

      // Check initial actions is 1
      #if (NOISY_TEST == 1)
        printf("Check initial actions is 1\n");
      #endif
      r = initializeGame(numPlayer, k, seed, &G);
      count += asserttrue(G.numActions, 1); // check if the number of actions is correct
      total++;

      // Check initial buys is 1
      #if (NOISY_TEST == 1)
        printf("Check initial buys is 1\n");
      #endif
      r = initializeGame(numPlayer, k, seed, &G);
      count += asserttrue(G.numBuys, 1); // check if the number of buys is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
