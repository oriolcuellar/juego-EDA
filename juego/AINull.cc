#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Null


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

 /*void alrededor(vector <pair<int, int> > &v_pos,vector < vector <casilla> > &m_unico , int i, int j, int id){
      //marca las casillas de al rededor de una posicion.
      //dependiendo de lo que haya la visita o la marca invisitable
      if ( pos_ok(i+1, j) and m_unico[i+1][j].cost==-1 and m_unico[i+1][j].soy!="edificio"  and m_unico[i+1][j].soy!="persona"   and m_unico[i+1][j].soy!="huir"){
        v_pos.push_back(make_pair(i+1, j));
        m_unico[i+1][j].cost=m_unico[i][j].cost+1;
        m_unico[i+1][j].dir="arriba";
        if (cell(i+1, j).resistance!=-1 and cell(i+1, j).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i+1, j).resistance/turnos;
          turnos=turnos+1;
          m_unico[i+1][j].cost=m_unico[i+1][j].cost+turnos;
        }
      }
        if (pos_ok(i-1, j) and m_unico[i-1][j].cost==-1 and m_unico[i-1][j].soy!="edificio" and m_unico[i-1][j].soy!="persona" and m_unico[i-1][j].soy!="huir"){
        v_pos.push_back(make_pair(i-1, j));
        m_unico[i-1][j].cost=m_unico[i][j].cost+1;
        m_unico[i-1][j].dir="abajo";
        if (cell(i-1, j).resistance!=-1 and cell(i-1, j).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i-1, j).resistance/turnos;
          turnos=turnos+1;
          m_unico[i-1][j].cost=m_unico[i-1][j].cost+turnos;
        }
      }
      if (pos_ok(i, j+1) and m_unico[i][j+1].cost==-1 and m_unico[i][j+1].soy!="edificio" and m_unico[i][j+1].soy!="persona" and m_unico[i][j+1].soy!="huir"){
        v_pos.push_back(make_pair(i, j+1));
        m_unico[i][j+1].cost=m_unico[i][j].cost+1;
        m_unico[i][j+1].dir="izquierda";
        if (cell(i, j+1).resistance!=-1 and cell(i, j+1).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i, j+1).resistance/turnos;
          turnos=turnos+1;
          m_unico[i][j+1].cost=m_unico[i][j+1].cost+turnos;
        }
      }
      if (pos_ok(i, j-1) and m_unico[i][j-1].cost==-1 and m_unico[i][j-1].soy!="edificio" and m_unico[i][j-1].soy!="persona" and m_unico[i][j-1].soy!="huir"){
        v_pos.push_back(make_pair(i, j-1));
        m_unico[i][j-1].cost=m_unico[i][j].cost+1;
        m_unico[i][j-1].dir="derecha";
        if (cell(i, j-1).resistance!=-1 and cell(i, j-1).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i, j-1).resistance/turnos;
          turnos=turnos+1;
          m_unico[i][j-1].cost=m_unico[i][j-1].cost+turnos;
        }
      }
  }*/
