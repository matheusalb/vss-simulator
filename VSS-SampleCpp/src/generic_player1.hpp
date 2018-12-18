#ifndef GENERIC_PLAYER1_H
#define GENERIC_PLAYER1_H

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <math.h>
#include <iostream>
#include "utils.hpp"


/**
 * @brief Guarda a lógica de funcionamento de um jogador
 * 
 */



/*--------------------- GOLEIRO --------------------- */






class GenericPlayer1 {
    private:
        vss::TeamType _teamType;

    public:

        GenericPlayer1(vss::TeamType type) {
            this->_teamType = type;
        }

        /**
         * @brief Função chamada a cada atualização com informações novas
         * 
         * @param state Estado atual do jogo
         * @param index Indice do robo que se deseja atualizar 
         * @return vss::WheelsCommand Comandos para as rodas do robo que se deseja atualizar
         */
        vss::WheelsCommand update(vss::State state, int index) {
            Utils::Posture objective = defineObjective(state, index);
            return motionControl(state, objective, index);
        }

        /**
         * @brief Define a postura final que se deseja chegar na iteração atual
         * 
         * @param index Indice do robo que se deseja atualizar 
         * @return Posture Posiçãoque se deseja adquirir
         */
        float x,y,ang;
        Utils::Posture defineObjective(vss::State state2, int index)
        {

            if(this->_teamType == vss::TeamType::Blue){ //Se foi escolhido o time azul

                //mantendo o robô na área do goleiro
                if(state2.teamBlue[index].x > 24)
                    return Utils::Posture(15.5, state2.teamBlue[index].y, M_PI/4.);
                else if(state2.teamBlue[index].y > 100 || state2.teamBlue[index].y < 30)
                    return Utils::Posture(state2.teamBlue[index].x,65.0, M_PI/4.);
                else{ //se o goleiro tá na área...
                    
                    if(state2.ball.x<= 85.6){
                        //a bola tá na minha área do campo...
                        if(state2.ball.y < 43){
                            x = 15.5;
                            y = 40;
                            ang = M_PI/2.0;
                        }
                        else if(state2.ball.y > 84){
                            x = 15.5;
                            y = 84.;
                            ang = M_PI/2.0;
                        }
                        else{
                            x = 15.5;
                            y = state2.ball.y;
                            ang = M_PI/2.;
                        }
                    }
                    else{
                        x = 15.5;
                        y = 65;
                        ang = M_PI/2.0;
                    }
                    return Utils::Posture(x,y,ang);
                }
            }
            else{ //time amarelo...

                //mantendo o robô na área do goleiro
                if(state2.teamYellow[index].x < 145.5)
                    return Utils::Posture(155., state2.teamYellow[index].y, M_PI/4.);
                else if(state2.teamYellow[index].y > 100. || state2.teamYellow[index].y < 30)
                    return Utils::Posture(state2.teamYellow[index].x,65.0, M_PI/2.);
                else{ //se o goleiro tá na área...
                    
                    if(state2.ball.x >= 85.6){
                        //a bola tá na minha área do campo...
                        if(state2.ball.y < 43){
                            x = 155.;
                            y = 40.;
                            ang = M_PI/2.0;
                        }
                        else if(state2.ball.y > 84){
                            x = 155.;
                            y = 84.;
                            ang = M_PI/2.0;
                        }
                        else{
                            x = 155.;
                            y = state2.ball.y;
                            ang = M_PI/2.;
                        }
                    }
                    else{
                        x = 155.;
                        y = 65.;
                        ang = M_PI/2.0;
                    }
                    return Utils::Posture(x,y,ang);
                }
            }

            // Retorne o objetivo aqui
            //return Utils::Posture(85., 65.0, M_PI/4.);
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