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
  // arrays of all estates and not estates
  int coppers[MAX_HAND];
  int silvers[MAX_HAND];
  int golds[MAX_HAND];
  int non_treasure[MAX_HAND];
  for(i = 0; i < MAX_HAND; i++){
    coppers[i] = copper;
    silvers[i] = silver;
    golds[i] = gold;
    non_treasure[i] = curse;
  }

  printf ("TESTING _mine_effect():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.handCount[p] = handCount;                 // set the number of cards on hand

      // Check that the hand count has not changed
      #if (NOISY_TEST == 1)
        printf("Check that the first copper is now a silver\n");
      #endif
      memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
      temp = G.handCount[p];
      int choice[] = {0, silver, 0};
      _mine_effect(choice, 3, &G);
      count += asserttrue(G.handCount[p], temp); // check if the number of coins is correct
      total++;

      // Check that the first copper is now a silver
      #if (NOISY_TEST == 1)
        printf("Check that the first copper is now a silver\n");
      #endif
      memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
      choice[0] = 0;
      choice[1] = silver;
      choice[2] = 0;
      _mine_effect(choice, 3, &G);
      count += asserttrue(G.hand[p][0], silver); // check if the number of coins is correct
      total++;

      // Check that the first silver is now a gold
      #if (NOISY_TEST == 1)
        printf("Check that the first silver is now a gold\n");
      #endif
      memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to copper
      choice[0] = 0;
      choice[1] = gold;
      choice[2] = 0;
      _mine_effect(choice, 3, &G);
      count += asserttrue(G.hand[p][0], gold); // check if the number of coins is correct
      total++;

      // Check that the first gold is now a gold
      #if (NOISY_TEST == 1)
        printf("Check that the first gold is now a gold\n");
      #endif
      memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to copper
      choice[0] = 0;
      choice[1] = gold;
      choice[2] = 0;
      _mine_effect(choice, 3, &G);
      count += asserttrue(G.hand[p][0], gold); // check if the number of coins is correct
      total++;

      // Check that the first card has not changed
      #if (NOISY_TEST == 1)
        printf("Check that the first card has not changed\n");
      #endif
      memcpy(G.hand[p], non_treasure, sizeof(int) * handCount); // set all the cards to copper
      choice[0] = 0;
      choice[1] = copper;
      choice[2] = 0;
      _mine_effect(choice, 3, &G);
      count += asserttrue(G.hand[p][0], curse); // check if the number of coins is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
