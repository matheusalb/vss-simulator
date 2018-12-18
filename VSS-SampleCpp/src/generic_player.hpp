#ifndef GENERIC_PLAYER_H
#define GENERIC_PLAYER_H
#define APROXIMA 0
#define DECIDE_DESVIO 1
#define SOBE 2
#define DESCE 3
#define VOLTA 4

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"

/*------------------- ATACANTE --------------------- */ 



/**
 * @brief Guarda a lógica de funcionamento de um jogador
 * 
 */
class GenericPlayer {
    private:
        vss::TeamType _teamType;

    public:

        GenericPlayer(vss::TeamType type) {
            this->_teamType = type;
        }

        /**
         * @brief Função chamada a cada atualização com informações novas
         * 
         * @param state Estado atual do jogo
         * @param index Indice do robo que se deseja atualizar 
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */

        //ALTERAR ESSA FUNÇÃO ---------------------------
        //retorna a velociddae pro robo de índice index
        vss::WheelsCommand update(vss::State state, int index) { //index == índice do robô
            Utils::Posture objective = defineObjective(state, index);
            return motionControl(state, objective, index);
        }

        /**
         * @brief Define a postura final que se deseja chegar na iteração atual
         * 
         * @param index Indice do robo que se deseja atualizar 
         * @return Posture Posiçãoque se deseja adquirir
         */

        //E ESSA -------------------------
        //define objetivo
        //int fazendo_manobra = 0;
        int estado = APROXIMA;
        int estadoant;
        float x,y,ang;
        float x_in,y_in;
        Utils::Posture defineObjective(vss::State state2, int index)
        {    
            if(this->_teamType == vss::TeamType::Blue){ //Se foi escolhido o time azul
                
                float distancia = sqrt( pow(state2.teamBlue[index].x - state2.ball.x,2) + pow(state2.teamBlue[index].y - state2.ball.y,2) );
                //calculo da distancia entre o jogador e a bola, por meio do módulo do vetor diferença!
                
                if( state2.teamBlue[index].x < state2.ball.x ){ //se o robô está atrás da bola
                    estado = APROXIMA;

                    if( (state2.teamBlue[index].y < state2.ball.y - 5 || state2.teamBlue[index].y > state2.ball.y + 5) && state2.teamBlue[index].x >= 22)
                        //alinhando o robô com a bola!
                        return Utils::Posture(state2.ball.x - 10,state2.ball.y, M_PI/4.); // x,y,angle
                    
                    else if( distancia < 8 && 49 < state2.teamBlue[index].y && 83 > state2.teamBlue[index].y){
                        //Se o robô 'está' com a bola e está indo em direção ao gol
                        return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.);
                    }
                    else if( distancia < 8 && (43 > state2.teamBlue[index].y || 83 < state2.teamBlue[index].y)){
                        //Se o robô 'está' com a bola e NÃO está indo em direção ao gol
                        return Utils::Posture(state2.ball.x, 66, M_PI/4.);
                    } 
                    else
                        return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.);
                        //no mais, corre atrás da bola...
                }
                else{
                    switch(estado){
                        case APROXIMA:
                            //corre atras da bola ate chegar perto
                            if(distancia < 10)
                                estado = DECIDE_DESVIO;
                            x=state2.ball.x + 10;
                            y= state2.ball.y;
                            ang = 0; // x,y,angle
                        break;
                        
                        case DECIDE_DESVIO:
                            //desvia da bola, para voltar a ficar atrás dela
                            if(state2.teamBlue[index].y - 10 > 6)
                                estado = SOBE;
                            else
                                estado = DESCE;

                        break;

                        case SOBE:
                            estadoant = SOBE;
                            estado = VOLTA;
                            x = state2.ball.x + 10;
                            y = state2.ball.y - 8;
                            x_in = state2.teamBlue[index].x;
                            y_in = state2.teamBlue[index].y;
                            ang = M_PI/2.0; // x,y,angle
        
                        break;
                        case DESCE:
                            estadoant = DESCE;
                            estado = VOLTA;
                            x = state2.ball.x + 10;
                            y = state2.ball.y + 8;
                            x_in = state2.teamBlue[index].x;
                            y_in = state2.teamBlue[index].y;
                            ang = M_PI/2.0; // x,y,angle
                        
                        break;
                        case VOLTA:
                            if(state2.teamBlue[index].x <= x_in - 16 || state2.teamBlue[index].x <= 12){
                                //se ele andou 16 ou tá no limite do mapa
                                //fazendo_manobra = 0;
                                estado = APROXIMA;
                            }
                            if(estadoant == DESCE){
                            x = state2.ball.x - 16;
                            y = state2.ball.y + 8;
                            ang = M_PI; // x,y,angle */
                            }
                            else{
                                x = state2.ball.x - 16;
                                y = state2.ball.y - 8;
                                ang = M_PI; // x,y,angle */
                            }
                        break;
                    }
                    /*if(state2.teamBlue[index].x > state2.ball.x + 10){
                        return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.); // x,y,angle
                    }
                    else if(state2.teamBlue[index].y <= state2.ball.y+10){
                        return Utils::Posture(state2.ball.x + 10, state2.ball.y + 5, M_PI/4.); // x,y,angle
                    }
                    else if(state2.teamBlue[index].y >= state2.ball.y + 5){
                        return Utils::Posture(state2.ball.x - 10, state2.ball.y + 5, M_PI/4.); // x,y,angle 
                    // fazendo_manobra = 0;
                    }
                    else
                        return Utils::Posture(state2.teamBlue[index].x, state2.teamBlue[index].y, M_PI/4.); // x,y,angle */
                    return Utils::Posture( x, y, ang);
                }
            }
            else{ //foi escolhido o time amarelo

                float distancia = sqrt( pow(state2.teamYellow[index].x - state2.ball.x,2) + pow(state2.teamYellow[index].y - state2.ball.y,2) );
                //calculo da distancia entre o jogador e a bola, por meio do módulo do vetor diferença!
                
                if( state2.teamYellow[index].x > state2.ball.x ){ //se o robô está atrás da bola
                    estado = APROXIMA;

                    if( (state2.teamYellow[index].y < state2.ball.y - 5 || state2.teamYellow[index].y > state2.ball.y + 5) && state2.teamYellow[index].x <= 144.)
                        //alinhando o robô com a bola!
                        return Utils::Posture(state2.ball.x + 10,state2.ball.y, M_PI/4.); // x,y,angle
                    
                    else if( distancia < 8 && 49 < state2.teamYellow[index].y && 83 > state2.teamYellow[index].y){
                        //Se o robô 'está' com a bola e está indo em direção ao gol
                        return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.);
                    }
                    else if( distancia < 8 && (43 > state2.teamYellow[index].y || 83 < state2.teamYellow[index].y)){
                        //Se o robô 'está' com a bola e NÃO está indo em direção ao gol
                        return Utils::Posture(state2.ball.x, 66, M_PI/4.);
                    } 
                    else
                        return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.);
                        //no mais, corre atrás da bola...
                }
                else{
                    if(state2.teamYellow[index].x < 154){
                        switch(estado){
                            case APROXIMA:
                                //corre atras da bola ate chegar perto
                                if(distancia < 10)
                                    estado = DECIDE_DESVIO;
                                x= state2.ball.x - 10;
                                y= state2.ball.y;
                                ang = 0; // x,y,angle
                            break;
                            
                            case DECIDE_DESVIO:
                                //desvia da bola, para voltar a ficar atrás dela
                                if(state2.teamYellow[index].y - 10 > 6)
                                    estado = SOBE;
                                else
                                    estado = DESCE;

                            break;

                            case SOBE:
                                estadoant = SOBE;
                                estado = VOLTA;
                                x = state2.ball.x - 10;
                                y = state2.ball.y - 8;
                                x_in = state2.teamYellow[index].x;
                                y_in = state2.teamYellow[index].y;
                                ang = M_PI/2.0; // x,y,angle
            
                            break;
                            case DESCE:
                                estadoant = DESCE;
                                estado = VOLTA;
                                x = state2.ball.x - 10;
                                y = state2.ball.y + 8;
                                x_in = state2.teamYellow[index].x;
                                y_in = state2.teamYellow[index].y;
                                ang = M_PI/2.0; // x,y,angle
                            
                            break;
                            case VOLTA:
                                if(state2.teamYellow[index].x >= x_in + 16 || state2.teamYellow[index].x >= 157.){
                                    //se ele andou 16 ou tá no limite do mapa
                                    //fazendo_manobra = 0;
                                    estado = APROXIMA;
                                }
                                if(estadoant == DESCE){
                                x = state2.ball.x + 16;
                                y = state2.ball.y + 8;
                                ang = M_PI; // x,y,angle */
                                }
                                else{
                                    x = state2.ball.x + 16;
                                    y = state2.ball.y - 8;
                                    ang = M_PI; // x,y,angle */
                                }
                            break;
                        }
                    }
                    else {
                        estado = APROXIMA;
                        x = state2.ball.x;
                        y = state2.ball.y;
                        ang = 0.;
                    }
                    return Utils::Posture( x, y, ang);
                
                }

            }

            // Retorne o objetivo aqui
           // return Utils::Posture(state2.ball.x, state2.ball.y, M_PI/4.); // x,y,angle
        }

        /**
         * @brief Traduz o objetivo desejado em velocidade das rodas utilizando o algoritmo Motion Control presente 
         * no livro Autonomous Mobile Robots, Siegwart
         * 
         * @param state Situação de jogo atual
         * @param objective Posição que se deseja chegar
         * @param index Indice do robo que se deseja atualizar
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */
        vss::WheelsCommand motionControl(vss::State state, Utils::Posture objective, int index)
        {
            vss::WheelsCommand result;
            double  alpha, beta, rho, lambda;
            double linearSpeed, angularSpeed;

            double kRho = 1.85, kAlpha = 9.7, kBeta = -0.01;

            vss::Robot robot = (this->_teamType == vss::TeamType::Blue) ? state.teamBlue[index] : state.teamYellow[index];

            robot.angle = Utils::to180range(robot.angle * M_PI / 180.0);

            rho = Utils::distance(doublePair(objective.x, objective.y), doublePair(robot.x, robot.y));

            lambda = atan2(objective.y - robot.y, objective.x - robot.x);

            if (rho < 3)
                lambda = 0;

            alpha = Utils::to180range(lambda - robot.angle);
            beta = -lambda + objective.angle;

            linearSpeed = -kRho * rho;
            angularSpeed = kAlpha * alpha + kBeta * beta;

            double constantSpeed = 60;
            double scale = constantSpeed / linearSpeed;
            linearSpeed *= scale;
            angularSpeed *= scale;

            if (rho < 3){
                linearSpeed = 0;
                angularSpeed *= 0.4;
            }
                
            if (fabs(alpha) > 0.5 * M_PI) {
                linearSpeed = -linearSpeed;
            }

            result.leftVel = (linearSpeed - angularSpeed * 3.35) / 2.;
            result.rightVel = (linearSpeed + angularSpeed * 3.35) / 2.;

            double maxSpeed = std::max(fabs(result.leftVel), fabs(result.rightVel));

            if (maxSpeed > 100) {
                result.leftVel *= 100 / fabs(maxSpeed);
                result.rightVel *= 100 / fabs(maxSpeed);
            }

            return result;
        }

};

#endif