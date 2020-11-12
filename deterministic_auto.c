#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  int cur, num_of_states, num_of_symbols = 0, file, bytes, num_of_fstates = 0, file_line;
  char *symbols, *final_states, *transitions, buffer[2], cur_state, start_state;

  char usr_input[1000];
  char answer[2] = "y";

  if ((file = open(argv[1],O_RDONLY)) == -1){
      printf("\nThere is no such file!!!\n\n");
      exit(0);
  }

  if (lseek(file,0,SEEK_END) == 0) {
    printf("%s is empty!\n",argv[1] );
    exit(0);
  }

  lseek(file,2,SEEK_SET);

  do {

    bytes = read(file,buffer,1);
    buffer[1] = '\0';
    //printf("bytes = %d\na = %s\n",bytes,a);

    if (strcmp(buffer," ") != 0 && strcmp(buffer,"\n") != 0)
      num_of_symbols++;

  } while(strcmp(buffer,"\n") != 0);

  cur = 2 + 2 * num_of_symbols + 2;
  lseek(file,cur,SEEK_SET);

  do {

    bytes = read(file,buffer,1);
    buffer[1] = '\0';
    //printf("bytes = %d\na = %s\n",bytes,a);

    if (strcmp(buffer," ") != 0 && strcmp(buffer,"\n") != 0)
      num_of_fstates++;

  } while(strcmp(buffer,"\n") != 0);

  symbols = (char*)malloc(num_of_symbols * sizeof(char));
  final_states = (char*)malloc(num_of_fstates * sizeof(char));

  int symbol = 0;
  int state = 0;
  file_line = 1;

  lseek(file,0,SEEK_SET);

  do {
    bytes = read(file,buffer,1);
    buffer[1] = '\0';
    //printf("bytes = %d\na = %s\n",bytes,buffer);

    if ( (strcmp(buffer,"\n")) == 0){
      file_line++;
      if (file_line > 4)
        break;
      continue;
    }

    if ( (strcmp(buffer," ")) != 0){

      switch (file_line) {
        case 1:
          num_of_states = atoi(buffer);
          break;
        case 2:
          symbols[symbol] = buffer[0];
          symbol++;
          break;
        case 3:
          start_state = buffer[0];
          cur_state = start_state;
          break;
        case 4:
          final_states[state] = buffer[0];
          state++;
          break;
        default:
          printf("DEFAULT!\n");
      }

    }

  }while (1);

  printf("\n*******************************************************************\n");
  printf("***                                                             ***\n");
  printf("***                            DFA                              ***\n");
  printf("***                                                             ***\n");
  printf("*******************************************************************\n\n");
  printf("[Total states: %d] ",num_of_states);
  printf("[Total symbols: %d] ",num_of_symbols );
  printf("[Final states: %d]\n",num_of_fstates );
  printf("Symbols: ");

  for (int i = 0; i < num_of_symbols; i++) {

    if (i+1 == num_of_symbols) {
      printf("%c]\n",symbols[i]);
    }else if(i == 0){
      printf("[%c,",symbols[i]);
    }else{
      printf("%c,",symbols[i]);
    }

  }

  printf("Final states: ");

  for (int i = 0; i < num_of_fstates; i++) {

    if (i+1 == num_of_fstates) {
      printf("%c]\n",final_states[i]);
    }else if(i == 0){
      printf("[%c,",final_states[i]);
    }else{
      printf("%c,",final_states[i]);
    }

  }

  printf("Start state: %c\n",start_state);
  printf("Current state: %c\n",cur_state);

  transitions = (char*)malloc(num_of_states * num_of_symbols * 3);

  int transition = 0;

  cur = 2 + 2 * num_of_symbols + 2 + 2 * num_of_fstates;
  lseek(file,cur,SEEK_SET);

  file_line = 1;

  do {
    bytes = read(file,buffer,1);
    buffer[1] = '\0';
    //printf("bytes = %d\na = %s\n",bytes,a);

    if ( (strcmp(buffer,"\n")) == 0){
      file_line++;
      if (file_line > num_of_states*num_of_symbols)
        break;
      continue;
    }

    if ( (strcmp(buffer," ")) != 0){

      transitions[transition] = buffer[0];
      transition++;
    }
  }while (1);

  /*for (int i = 0; i < num_of_states * num_of_symbols * 3; i++)
    printf("transitions[%d]: %c\n",i,transitions[i]);*/

  int next_state,word_accepted,input,cur_state_accepted;

  while ((strcmp(answer,"n") != 0) && (strcmp(answer,"N") != 0)) {

    if (strcmp(answer,"Y") == 0 || strcmp(answer,"y") == 0) {

      input = 0;
      cur_state_accepted = 0;

      cur_state = start_state;

      printf("\nWord: ");
      scanf("%s",usr_input);

      for (int i = 0; i < num_of_states * num_of_symbols * strlen(usr_input); i++) {

        if (input+1 > strlen(usr_input))
          break;

        if (i % (num_of_states * num_of_symbols) == 0){
          state = 0;
          symbol = 1;
          next_state = 2;
          word_accepted = 0;
        }

        if (cur_state == transitions[state] && usr_input[input] == transitions[symbol]) {
          //printf("cur_state (%c) = transitions (%c)\n",cur_state,transitions[state] );
          //printf("usr_input (%c) = transitions (%c)\n",usr_input[input],transitions[symbol] );
          printf("\n[ Current state: %c , input: %c ] => next state: ", cur_state,usr_input[input]);
          cur_state = transitions[next_state];
          //printf("Current state: %c\n", cur_state);
          printf("%c\n", cur_state);
          input++;
          word_accepted = 1;
          continue;
        }

        state += 3;
        symbol += 3;
        next_state += 3;

      }

      if (word_accepted == 0){
          printf("\nNot accepted word!\nElement (%c) doesn't belong to the alphabet.\n",usr_input[input]);
      }else{

        for (int i = 0; i < strlen(final_states); i++) {

          if (cur_state == final_states[i]) {
            cur_state_accepted = 1;
            break;
          }

        }

        if (cur_state_accepted == 0) {
          printf("\nValid word, but state (%c) is not a valid final state.\n",cur_state);
        }else
          printf("\nAccepted word.\n");

      }

    }

    printf("\nDo you want to insert another word? (Y/n): ");
    scanf("%s", answer);
  }

  free(symbols);
  free(final_states);
  free(transitions);
  close(file);

  return 0;
}
