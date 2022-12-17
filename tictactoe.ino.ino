
#include <Wifi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h> //mqtt
#include <time.h>
#include <stdlib.h>

//MQTT
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port = 1883;
#define DEVICE_MQTT_NAME "tictactoe"


WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, espClient);

const char* ssid = ""; // Wifi network SSID
const char* password = ""; // Wifi network password

srand(time(NULL));//for random seed
    
char piece;//the piece to be placed in char board[10]
int player = 1;//tracks player's turn
int turnCounter = 1;//tracks how many turns have elapsed
int winStatus = 0;//tracks win conditions
int selection;//user selection
int random;//used to generate random move by computer
int emptySpace = 0;//used to help track valid moves


void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid_Router, password_Router);
  while (WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  irrecv.enableIRIn();        // Start the receiver
  Serial.println("IR Receiver ready");
}


void loop() {

  resetBoard();//reset the board each play
        Serial.println("\nIs a human present?");
        Serial.println("\n(1 - no, 2 - yes): ");
        if (Serial.available()){//for the script
          choice = Serial.read();
        }
        choice = getInput();
        //start the game
        Serial.println("\nStarting game......\n");
        playGame(choice);
}


char board[10];//the board used for the game
int winCondition();//check if a win condition is met
void displayBoard();//print board to console
int boardTracker[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};//tracks unused positions, used for computer player

//display the board
void displayBoard(){
    Serial.println("\n     |     |     \n");
    Serial.println("  %c  |  %c  |  %c \n", board[1], board[2], board[3]);
    Serial.println("_ _ _|_ _ _|_ _ _\n");
    Serial.println("     |     |     \n");
    Serial.println("  %c  |  %c  |  %c \n", board[4], board[5], board[6]);
    Serial.println("_ _ _|_ _ _|_ _ _\n");
    Serial.println("     |     |     \n");
    Serial.println("  %c  |  %c  |  %c \n", board[7], board[8], board[9]);

    Serial.println("     |     |     \n\n");
}

//reset the board
void resetBoard(){
    //reset the board to this
    char baseBoard[10] = {'-', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for(int i = 0; i < 10; i++){
        board[i] = baseBoard[i];
        boardTracker[i] = i;
    }
}

//check win conditions
int winCondition(){

    if (board[1] == board[2] && board[2] == board[3] ||//across
        board[4] == board[5] && board[5] == board[6] ||
        board[7] == board[8] && board[8] == board[9] ||
        board[1] == board[4] && board[4] == board[7] ||//down
        board[2] == board[5] && board[5] == board[8] ||
        board[3] == board[6] && board[6] == board[9] ||
        board[1] == board[5] && board[5] == board[9] ||//diag
        board[3] == board[5] && board[5] == board[7]
        )
        return 1;
    else
        return  0;
}

//play game
void playGame(int computer){

    srand(time(NULL));//for random seed
    
    char piece;//the piece to be placed in char board[10]
    int player = 1;//tracks player's turn
    int turnCounter = 1;//tracks how many turns have elapsed
    int winStatus = 0;//tracks win conditions
    int selection;//user selection
    int random;//used to generate random move by computer
    int emptySpace = 0;//used to help track valid moves
    

    do{
        displayBoard();
        player = (player % 2) ? 1 : 2;

        if(player == 2 && computer == 1){//check computer boolean
            do{//generate random, valid move
                random = rand()%10;
                if(boardTracker[random] != 0){
                    selection = boardTracker[random];
                    Serial.println("Player 2 (COMPUTER) places a piece in position: %d", selection);
                    emptySpace++;
                }
            }while (emptySpace == 0);

            emptySpace = 0;
            
        }else{//get move selection from user
            Serial.println("Enter a number on the board, player %d: ", player);
            selection = getInput();
        }

        piece = (player == 1) ? 'X' : 'O';//set piece according to user

        if(boardTracker[selection] == 0){//validates move
            Serial.println("That spot is taken, please try again: ");
            player--;
        }else{
            board[selection] = piece;
            boardTracker[selection] = 0;
            turnCounter++;
        }


        player++;
        winStatus = winCondition();
    }while(winStatus == 0 && turnCounter < 10);

    displayBoard();

    if(winStatus == 1){//checks win conditions
        Serial.println("GAME OVER: Player %d wins!", --player);
    }else{
        Serial.println("GAME OVER: DRAW!");
    }

  int getInput(){
    int x;
    int breaker = 1;
    while(1){
      if (irrecv.decode(&results)) { 
        x = results.value;
        irrecv.resume();
      }

      switch(x){
        case 16724175 :
          return 1;
        case 16718055 :
          return 2;
        case 16743045 :
          return 3;
        case 16716015 :
          return 4;
        case 16726215 :
          return 5;
        case 16734885 :
          return 6;
        case 16728765 :
          return 7;
        case 16730805 :
          return 8;
        case 16732845 :
          return 9;
        default :
          Serial.println("Enter a number between 1 and 9 on the keypad");

      }
      delay(1000);
    }
  }
}







