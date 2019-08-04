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
  int treasures[MAX_DECK];
  int actions[MAX_DECK];
  int victories[MAX_DECK];
  int curses[MAX_DECK];
  for(i = 0; i < MAX_DECK; i++){
    treasures[i] = copper;
    actions[i] = tribute;
    victories[i] = estate;
    curses[i] = curse;
  }
  int tributeRevealedCards[2] = {-1, -1};

  printf ("TESTING _tribute_effect():\n");
  for (p = 0; p < numPlayer; p++)
  {
    for (handCount = 1; handCount <= maxHandCount; handCount++)
    {
      printf("Test player %d with %d card(s) and %d bonus.\n", p, handCount, bonus);
      memset(&G, 23, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      G.handCount[p] = handCount;                 // set the number of cards on hand

      // Check that coins get +2
      #if (NOISY_TEST == 1)
        printf("Check that coins get +2\n");
      #endif
      memcpy(G.deck[p], treasures, sizeof(int) * G.deckCount[p]); // set all the cards to copper
      temp = G.coins;
      int choice[] = {0};
      if(p == 0) _tribute_effect(choice, 0, &G);
      else if(p == 1) _tribute_effect(choice, 0, &G);
      count += asserttrue(G.coins, temp + 2); // check if the number of coins is correct
      total++;

      // Check that actions get +2
      #if (NOISY_TEST == 1)
        printf("Check that actions get +2\n");
      #endif
      memcpy(G.deck[p], actions, sizeof(int) * G.deckCount[p]); // set all the cards to copper
      temp = G.numActions;
      choice[0] = 0;
      if(p == 0) _tribute_effect(choice, 0, &G);
      else if(p == 1) _tribute_effect(choice, 0, &G);
      count += asserttrue(G.numActions, temp + 2); // check if the number of actions is correct
      total++;

      // Check that player get +2 cards
      #if (NOISY_TEST == 1)
        printf("Check that player get +2 cards\n");
      #endif
      memcpy(G.deck[p], victories, sizeof(int) * G.deckCount[p]); // set all the cards to copper
      temp = G.handCount[p];
      choice[0] = 0;
      if(p == 0) _tribute_effect(choice, 0, &G);
      else if(p == 1) _tribute_effect(choice, 0, &G);
      count += asserttrue(G.handCount[p], temp + 2); // check if the number of cards is correct
      total++;

      // Check that there is no change in coins
      #if (NOISY_TEST == 1)
        printf("Check that there is no change in coins\n");
      #endif
      memcpy(G.deck[p], curses, sizeof(int) * G.deckCount[p]); // set all the cards to copper
      temp = G.coins;
      choice[0] = 0;
      if(p == 0) _tribute_effect(choice, 0, &G);
      else if(p == 1) _tribute_effect(choice, 0, &G);
      count += asserttrue(G.coins, temp); // check if the number of coins is correct
      total++;
    }
  }

  printf("%d out of %d passed.\n", count, total);

  return 0;
}
