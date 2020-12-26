#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Oriolcuellar






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
  struct casilla{
      string dir;//derecha, abajo, izquierda, arriba
      int cost;//-1 inexplorado, distancia hasta el origen
      string soy;//suelo, edificio, enemigo, amigo, pistola, bazoca, barricada
  };

  struct accion{
    Pos p;
    string obj;
    string dir;
    int id;
    string soy;
  };


//crear mapa
  void mapa(bool noche, vector < vector <casilla> > &mat,vector<pair <int, int> > &v_enemigo,vector<pair <int, int> > &v_bazooka,vector<pair <int, int> > &v_food,vector<pair <int, int> > &v_pistola,vector<pair <int, int> > &v_money){
    int filas=board_rows();
    int columnas=board_cols();

    for (int j=0;j<columnas;j++){
      for (int i=0;i<filas;i++){
        mat[i][j].cost=-1;
        if (cell(i, j).type==Street) mat[i][j].soy="suelo";
        else mat[i][j].soy="edificio";
        if (cell(i, j).id!=-1){
          int id=cell(i, j).id;
          int jugador=citizen(id).player;
          mat[i][j].soy="persona";
          if (jugador!=me()) mat[i][j].soy="enemigo";
          
        }
        if (cell(i,j).weapon==Bazooka) mat[i][j].soy="bazooka";
        if (cell(i,j).weapon==Gun) mat[i][j].soy="pistola";
        if (cell(i,j).bonus==Money) mat[i][j].soy="money";
        if (cell(i,j).bonus==Food) mat[i][j].soy="food";
        if (cell(i,j).resistance!=-1 and cell(i,j).b_owner!=me() and mat[i][j].soy!="enemigo") mat[i][j].soy="barricada";

        if (is_day() and mat[i][j].soy=="barricada") mat[i][j].soy="edificio";
        if ((not noche) and mat[i][j].soy=="enemigo" ) mat[i][j].soy="edificio";//si es dia no podemos mata ni tirar barricadas

        if (mat[i][j].soy=="pistola") v_pistola.push_back(make_pair(i, j));
        if (mat[i][j].soy=="bazooka") v_bazooka.push_back(make_pair(i, j));
        if (mat[i][j].soy=="money") v_money.push_back(make_pair(i, j));
        if (mat[i][j].soy=="food") v_food.push_back(make_pair(i, j));
        if (mat[i][j].soy=="enemigo") v_food.push_back(make_pair(i, j));
      }
    }
  }

  string buscar_camino(const vector < vector <casilla> > &matriz, int i, int j){//sin hacer----------------------------------------------------
    bool trobat=false;
    string dir;
    while (trobat==false){
          if(matriz[i][j].dir=="derecha"){
            if(matriz[i][j+1].cost==0){
              trobat=true;
              dir="derecha";
            }
            j++;
          }
          else if(matriz[i][j].dir=="izquierda"){
            if(matriz[i][j-1].cost==0){
              trobat=true;
              dir="izquierda";
            }
            j--;
          }
          else if(matriz[i][j].dir=="abajo"){
            if(matriz[i+1][j].cost==0){
              trobat=true;
              dir="abajo";
            }
            i++;
          }
          else if(matriz[i][j].dir=="arriba"){
            if(matriz[i-1][j].cost==0){
              trobat=true;
              dir="arriba";
            }
            i--;
          }
    }
    return dir;

  }
  void alrededor(vector <pair<int, int> > &v_pos,vector < vector <casilla> > &m_unico , int i, int j, int id){
      //marca las casillas de al rededor de una posicion.
      //dependiendo de lo que haya la visita o la marca invisitable
      if ( pos_ok(i+1, j) and m_unico[i+1][j].soy!="edificio"  and m_unico[i+1][j].soy!="persona"   and m_unico[i+1][j].soy!="huir"){

        int dist_nueva=m_unico[i][j].cost+1;

        if (cell(i+1, j).resistance!=-1 and cell(i+1, j).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i+1, j).resistance/turnos;
          turnos=turnos+1;
          dist_nueva=dist_nueva+turnos;
        }
        if (dist_nueva<m_unico[i+1][j].cost or m_unico[i+1][j].cost==-1){
          v_pos.push_back(make_pair(i+1, j));
          m_unico[i+1][j].cost=dist_nueva;
          m_unico[i+1][j].dir="arriba";
        }
      }
        if (pos_ok(i-1, j)  and m_unico[i-1][j].soy!="edificio" and m_unico[i-1][j].soy!="persona" and m_unico[i-1][j].soy!="huir"){

        int dist_nueva=m_unico[i][j].cost+1;

        if (cell(i-1, j).resistance!=-1 and cell(i-1, j).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i-1, j).resistance/turnos;
          turnos=turnos+1;
          dist_nueva=dist_nueva+turnos;
        }
        if (dist_nueva<m_unico[i-1][j].cost or m_unico[i-1][j].cost==-1){
          v_pos.push_back(make_pair(i-1, j));
          m_unico[i-1][j].cost=dist_nueva;
          m_unico[i-1][j].dir="abajo";
        }
      }
      if (pos_ok(i, j+1)  and m_unico[i][j+1].soy!="edificio" and m_unico[i][j+1].soy!="persona" and m_unico[i][j+1].soy!="huir"){
        int dist_nueva=m_unico[i][j].cost+1;

        if (cell(i, j+1).resistance!=-1 and cell(i, j+1).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i, j+1).resistance/turnos;
          turnos=turnos+1;
          dist_nueva=dist_nueva+turnos;
        }
        if (dist_nueva<m_unico[i][j+1].cost or m_unico[i][j+1].cost==-1){
          v_pos.push_back(make_pair(i, j+1));
          m_unico[i][j+1].cost=dist_nueva;
          m_unico[i][j+1].dir="izquierda";
        }
      }
      if (pos_ok(i, j-1)  and m_unico[i][j-1].soy!="edificio" and m_unico[i][j-1].soy!="persona" and m_unico[i][j-1].soy!="huir"){
        int dist_nueva=m_unico[i][j].cost+1;

        if (cell(i, j-1).resistance!=-1 and cell(i, j-1).b_owner!=me()){
          int turnos=1;
          if (citizen(id).type==Builder){ turnos=builder_strength_demolish();}
          else if (citizen(id).weapon==Hammer){ turnos=hammer_strength_demolish();}
          else if (citizen(id).weapon==Gun){ turnos=gun_strength_demolish();}
          else if (citizen(id).weapon==Bazooka){ turnos=bazooka_strength_demolish();}
          if (turnos==0) turnos=1;
          turnos=cell(i, j-1).resistance/turnos;
          turnos=turnos+1;
          dist_nueva=dist_nueva+turnos;
        }
        if (dist_nueva<m_unico[i][j-1].cost or m_unico[i][j-1].cost==-1){
          v_pos.push_back(make_pair(i, j-1));
          m_unico[i][j-1].cost=dist_nueva;
          m_unico[i][j-1].dir="derecha";
        }
      }
  }
bool barricada_alrededor_enemigo(vector < vector <casilla> >  &m_unico, int i, int j, int id, int &res){
    if (pos_ok(i, j+1) and m_unico[i][j+1].soy=="barricada" and cell(i, j+1).b_owner==id){
      res=cell(i, j+1).resistance;
      return true;
    }
    else if (pos_ok(i+1, j) and m_unico[i+1][j].soy=="barricada" and cell(i+1, j).b_owner==id){
      res=cell(i+1, j).resistance;
      return true;
    }
    else if (pos_ok(i-1, j) and m_unico[i-1][j].soy=="barricada" and cell(i-1, j).b_owner==id){
      res=cell(i-1, j).resistance;
      return true;
    }
    else if (pos_ok(i, j-1) and m_unico[i][j-1].soy=="barricada" and cell(i, j-1).b_owner==id){
      res=cell(i, j-1).resistance;
      return true;
    }
  return false;
}
  long double determinar_valor_w(string soc, int dist, int warrior, int builder, int money, int mi_vida, int su_vida, int vida_total,vector < vector <casilla> >  &m_unico,int w, int i, int j){
    long double valor=-1;
    int quedan_rondas=(num_rounds_per_day()/2)-round()%(num_rounds_per_day()/2);

int dist_aux=0;

int k_builder=1200; //1000 
int k_warrior=1200; //1000 
int k_money=1000; //1000 
int k_food=1500; //1000 
int k_pistola=1000; //1000 
int k_bazooka=2000; //2000 
int k_no_bazooka=4000;//4000

    if (soc=="enemigo"){
      valor=warrior*k_warrior;
    } 
    else if (soc=="builder"){
      valor=builder*k_builder;
      int res=1;
      if (barricada_alrededor_enemigo(m_unico, i, j, citizen(cell(i, j).id).player, res)){
        
        int rondas=1;
        if (citizen(w).weapon==Hammer) rondas=res/hammer_strength_demolish();
        if (citizen(w).weapon==Gun) rondas=res/gun_strength_demolish();
        if (citizen(w).weapon==Bazooka) rondas=res/bazooka_strength_demolish();
        rondas+=2;
        dist_aux=rondas;

      }
    }
    else if (soc=="money") valor=money*k_money;
    else if (soc=="bazooka"){
      valor=builder*k_bazooka;
      if (citizen(w).weapon!=Bazooka) valor =warrior*k_no_bazooka;
      
    }
    else if (soc=="pistola"){
      valor=warrior*k_pistola;
    }
    else if (soc=="food" and mi_vida!=0 and mi_vida!=warrior_ini_life()){
      int div=0;
      valor=money/2;
      div=(mi_vida*100)/vida_total;
      if (div<60){
        valor=(((builder+warrior)*100*k_food)/2)/div;
      }
    }

    if (dist==0) dist=1;
    dist+=dist_aux;
    valor=(valor*1000)/(dist*dist);

    return valor;
  }


  long double determinar_valor_b(string soc, int dist, int mi_vida, int vida_total, int b){
    long double valor=-1;
    

int k_money=1500; //1000 
int k_food=2000; //1000 
int k_pistola=1000; //10 
int k_bazooka=2000; //100






    if (soc=="builder"){}
    else if(soc=="money") valor=money_points()*k_money;
    else if (soc=="bazooka"){
      valor=money_points()*k_bazooka;
    }
    else if (soc=="pistola"){
      valor=money_points()*k_pistola;
    }
    else if (soc=="food" and citizen(b).life!=builder_ini_life()){
      unsigned int div=0;
      valor=money_points()/2;
      div=(mi_vida*100)/vida_total;
      if (div<60){
        valor=(kill_builder_points()*k_food)/div;
      }
    }
    if (dist==0) dist=1;
   
    valor=(valor*1000)/(dist*dist);

    return valor;
  }




  bool huir_o_luchar(vector < vector <casilla> >  &m_unico, int id,int w,int i,int j){
    if (citizen(id).type!=Warrior) return true;
    bool puedo_ir=true;
    if (citizen(id).weapon==Bazooka and (citizen(w).weapon!=Bazooka or (citizen(w).life<citizen(id).life)))puedo_ir=false;
    if (citizen(id).weapon==Gun and (citizen(w).weapon==Hammer or (citizen(w).weapon==Gun and citizen(w).life<citizen(id).life))) puedo_ir=false;
    if (citizen(id).weapon==Hammer and citizen(w).weapon==Hammer and citizen(w).life<citizen(id).life) puedo_ir=false;

    return puedo_ir;
  }


//......................................................................................................................
//......................................................................................................................


  void gestor_de_acciones_globales(vector <pair <bool, map < long double, accion, greater<long double> > > > &acciones_globales){
    map < long double, accion, greater<long double> > :: iterator it;
    set <Pos> :: iterator jt;
    set <Pos> v_pos;
    for (unsigned int i=0;i<acciones_globales.size();i++){

      long double val_max=-1;
      int soy=0;
      bool escogido=false;
      Dir d;
      Pos p;
      for (unsigned int j=0;j<acciones_globales.size();j++){

        it=acciones_globales[j].second.begin();
        bool trobat=false;
        if (acciones_globales[j].first==false) it=acciones_globales[j].second.end();
        while ((not trobat) and it!=acciones_globales[j].second.end()){//buscar pos no ocupada

          if (v_pos.find(it ->second.p)==v_pos.end() and val_max< it->first){//actualizo mejor movimiento
            if (it -> second.dir =="arriba")d=Down;
            if (it -> second.dir =="abajo")d=Up;
            if (it -> second.dir =="derecha")d=Left;
            if (it -> second.dir =="izquierda")d=Right;
            val_max=it->first;
            soy=j;
            p=it->second.p;
            trobat=true;
          }
          else ++it;
        }
        if (trobat) escogido=true;
        
        
      }
      if (escogido){//actuar, masrcar pos, quitar del vect
        it=acciones_globales[soy].second.begin();
        acciones_globales[soy].first=false;
        soy=it->second.id;
        v_pos.insert(p);
        
        move(soy,d );
      }
    }
  }

//tengo que mejorar
  bool construir_barricada(vector <pair <bool, map < long double, accion, greater<long double> > > > &acciones_globales, vector < vector <casilla> >  &m_unico,int i,int j, int b){
    bool hecho=false;
    int barr=barricades(me()).size();
    if (cell(i,j).b_owner!=me() and max_num_barricades() > barr){//no estoy en barricada y quedan por construir 

    if (pos_ok(i+1, j) and cell(i+1, j).resistance==-1 and m_unico[i+1][j].soy=="suelo" and pos_ok(i+1, j+1) and cell(i+1, j+1).type==Building and pos_ok(i+1, j-1) and cell(i+1, j-1).type==Building){
      Dir d;
      d=Down;
      build (b,d);
      hecho=true;
    }
    if (pos_ok(i-1, j) and cell(i-1, j).resistance==-1  and m_unico[i-1][j].soy=="suelo" and pos_ok(i-1, j+1) and cell(i-1, j+1).type==Building and pos_ok(i-1, j-1) and cell(i-1, j-1).type==Building){
      Dir d;
      d=Up;
      build (b,d);
      hecho=true;
    }
    if (pos_ok(i, j+1) and cell(i, j+1).resistance==-1  and m_unico[i][j+1].soy=="suelo" and pos_ok(i+1, j+1) and cell(i+1, j+1).type==Building and pos_ok(i-1, j+1) and cell(i-1, j+1).type==Building){
      Dir d;
      d=Right;
      build (b,d);
      hecho=true;
    }
    if (pos_ok(i, j-1) and cell(i, j-1).resistance==-1  and m_unico[i][j-1].soy=="suelo" and pos_ok(i+1, j-1) and cell(i+1, j-1).type==Building and pos_ok(i-1, j-1) and cell(i-1, j-1).type==Building){
      Dir d;
      d=Left;
      build (b,d);
      hecho=true;
    }
    }
    return hecho;
    
  }
  


  void buscar_huida(vector < vector <casilla> >  &m_unico, Pos mi_pos , Pos &enemigo, int id){
    int i=mi_pos.i;
    int j=mi_pos.j;
    int x=i-enemigo.i;
    if (x<0)x=-x;

    int y=j-enemigo.j;
    if (y<0)y=-y;
    int dist =x+y;
    bool movido=false;
    Dir d;
    int gente=-1;
      if (pos_ok(i+1, j) and m_unico[i+1][j].soy!="edificio"  and m_unico[i+1][j].soy!="persona"  and m_unico[i+1][j].soy!="enemigo" and m_unico[i+1][j].soy!="barricada"  and m_unico[i+1][j].soy!="builder"){
        x=i+1-enemigo.i;
        if (x<0)x=-x;
        y=j-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist){
          movido=true;
            int cont_gente=0;
            if(pos_ok(i+2, j) and  m_unico[i+2][j].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i+1, j+1) and  m_unico[i+1][j+1].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i+1, j-1) and  m_unico[i+1][j-1].soy=="enemigo"){
              cont_gente++;
            }
            if (cont_gente<gente or gente==-1){
              gente=cont_gente;
              d=Down;
            }
        }
      }
      if (pos_ok(i-1, j) and m_unico[i-1][j].soy!="edificio" and m_unico[i-1][j].soy!="persona" and m_unico[i-1][j].soy!="enemigo" and m_unico[i-1][j].soy!="barricada"  and m_unico[i-1][j].soy!="builder"){
        x=i-1-enemigo.i;
        if (x<0)x=-x;
        y=j-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist){
          movido=true;
            int cont_gente=0;
            if(pos_ok(i-2, j) and  m_unico[i-2][j].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i-1, j+1) and  m_unico[i-1][j+1].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i-1, j-1) and  m_unico[i-1][j-1].soy=="enemigo"){
              cont_gente++;
            }
            if (cont_gente<gente or gente==-1){
              gente=cont_gente;
              d=Up;
            }
        }
      }
      if (pos_ok(i, j+1) and m_unico[i][j+1].soy!="edificio" and m_unico[i][j+1].soy!="persona" and m_unico[i][j+1].soy!="enemigo" and m_unico[i][j+1].soy!="baricada"  and m_unico[i][j+1].soy!="builder"){
        x=i-enemigo.i;
        if (x<0)x=-x;
        y=j+1-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist){
          movido=true;
            int cont_gente=0;
            if(pos_ok(i, j+2) and  m_unico[i][j+2].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i-1, j+1) and  m_unico[i-1][j+1].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i+1, j+1) and  m_unico[i+1][j+1].soy=="enemigo"){
              cont_gente++;
            }
            if (cont_gente<gente or gente==-1){
              gente=cont_gente;
              d=Right;
            }
        }
        
      }
      if (pos_ok(i, j-1) and m_unico[i][j-1].soy!="edificio" and m_unico[i][j-1].soy!="persona" and m_unico[i][j-1].soy!="enemigo" and m_unico[i][j-1].soy!="barricada"  and m_unico[i][j-1].soy!="builder"){
        x=i-enemigo.i;
        if (x<0)x=-x;
        y=j-1-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist){
          movido=true;
            int cont_gente=0;
            if(pos_ok(i, j-2) and  m_unico[i][j-2].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i-1, j-1) and  m_unico[i-1][j-1].soy=="enemigo"){
              cont_gente++;
            }
            if(pos_ok(i+1, j-1) and  m_unico[i+1][j-1].soy=="enemigo"){
              cont_gente++;
            }
            if (cont_gente<gente or gente==-1){
              gente=cont_gente;
              d=Left;
            }
        }
      }
      if (movido==true)move (id, d);
  }

  bool const_barricadas(vector < vector <casilla> >  &m_unico, int id){
    int i=citizen(id).pos.i;
    int j=citizen(id).pos.j;
    int barr=barricades(me()).size();
    if (is_night() or cell(i, j).b_owner==me()){}//estoy en barricada o ya es noche
    else{
      int llevan=round()%(num_rounds_per_day()/2);//quedan para noche
      int quedan=(num_rounds_per_day()/2)-llevan;
      if (quedan<=barricade_max_resistance()/barricade_resistance_step()){//construimos o mejoramos
        if (max_num_barricades() != barr){//quedan por construir
          if (pos_ok(i, j-1) and m_unico[i][j-1].soy=="suelo"){
            build(id, Left);
            return true;
          }
          else if (pos_ok(i, j+1) and m_unico[i][j+1].soy=="suelo"){
            build(id, Right);
            return true;
          }
          else if (pos_ok(i-1, j) and m_unico[i-1][j].soy=="suelo"){
            build(id, Up);
            return true;
          }
          else if (pos_ok(i+1, j) and m_unico[i+1][j].soy=="suelo"){
            build(id, Down);
            return true;
          }
        }
        else {//mejoramos
          if (pos_ok(i, j-1) and cell(i, j-1).b_owner==me()){
            build(id, Left);
            return true;
          }
          else if (pos_ok(i, j+1) and cell(i, j+1).b_owner==me()){
            build(id, Right);
            return true;
          }
          else if (pos_ok(i-1, j) and cell(i-1, j).b_owner==me()){
            build(id, Up);
            return true;
          }
          else if (pos_ok(i+1, j) and cell(i+1, j).b_owner==me()){
            build(id, Down);
            return true;
          }
        }
        
      }
    }
    return false;
  }
bool barricada_alrededor(vector < vector <casilla> >  &m_unico, int i, int j){
    if (pos_ok(i, j+1) and m_unico[i][j+1].soy=="suelo" and cell(i, j+1).b_owner==me()) return true;
    else if (pos_ok(i+1, j) and m_unico[i+1][j].soy=="suelo" and cell(i+1, j).b_owner==me()) return true;
    else if (pos_ok(i-1, j) and m_unico[i-1][j].soy=="suelo" and cell(i-1, j).b_owner==me()) return true;
    else if (pos_ok(i, j-1) and m_unico[i][j-1].soy=="suelo" and cell(i, j-1).b_owner==me()) return true;
  return false;
}
bool enemigo_alrededor(vector < vector <casilla> >  &m_unico, int i, int j){
  if (pos_ok(i, j) and (cell(i, j).id==-1 or m_unico[i][j].soy=="suelo") and cell(i, j).b_owner==-1 and cell(i, j).type==Street){
    if (pos_ok(i, j+1) and m_unico[i][j+1].soy=="enemigo") return true;
    else if (pos_ok(i+1, j) and m_unico[i+1][j].soy=="enemigo") return true;
    else if (pos_ok(i-1, j) and m_unico[i-1][j].soy=="enemigo") return true;
    else if (pos_ok(i, j-1) and m_unico[i][j-1].soy=="enemigo") return true;
  }
  else return true;
  return false;
}
bool escondite(vector < vector <casilla> >  &m_unico, int id, vector < vector <casilla> >  &mat){
  int i=citizen(id).pos.i;
  int j=citizen(id).pos.j;
  if (cell(i, j).b_owner==me()){//estoy en barricada
    if (not enemigo_alrededor(m_unico,i, j-1)){
      move(id, Left);
      mat[i][j].soy="suelo";
      return true;
    }
    else if (not enemigo_alrededor(m_unico,i, j+1)){
      move(id, Right);
      mat[i][j].soy="suelo";
      return true;
    }
    else if (not enemigo_alrededor(m_unico,i-1, j)){
      move(id, Up);
      mat[i][j].soy="suelo";
      return true;
    }
    else if (not enemigo_alrededor(m_unico,i+1,j)){
      move(id, Down);
      mat[i][j].soy="suelo";
      return true;
    }
  }
  else{//no estoy en barricada
    if (not barricada_alrededor(m_unico, i, j)) return false;//huye
    if (not enemigo_alrededor(m_unico, i, j)) return true; //te quedas quieto 
    if (pos_ok(i+1, j) and m_unico[i+1][j].soy!="persona" and cell(i+1, j).b_owner==me()){
      move(id, Down);
      mat[i+1][j].soy="persona";
      return true;
    }
    if (pos_ok(i-1, j) and m_unico[i-1][j].soy!="persona" and cell(i-1, j).b_owner==me()){
      move(id, Up);
      mat[i-1][j].soy="persona";
      return true;
    }
    if (pos_ok(i, j+1) and m_unico[i][j+1].soy!="persona" and cell(i, j+1).b_owner==me()){
      move(id, Right);
      mat[i][j+1].soy="persona";
      return true;
    }
    if (pos_ok(i, j-1) and m_unico[i][j-1].soy!="persona" and cell(i, j-1).b_owner==me()){
      move(id, Left);
      mat[i][j-1].soy="persona";
      return true;
    }
  }
  return false;
}



  //......................................................................................................................
//......................................................................................................................


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
	
	// If nearly out of time, do nothing.

 //gasto un 10% (6-7) en ... 14x30  250 rondas = 105000

    


    //turnos para noche
    bool noche=false;
    if (is_night()) noche=true;
    else{
      int llevan=round()%(num_rounds_per_day()/2);//quedan para noche
      int quedan=(num_rounds_per_day()/2)-llevan;
      if (quedan<6) noche=true;

    }



    //parametros
    int filas=board_rows();
    int columnas=board_cols();
    int movimientos=num_rounds_per_day()/2;
    int llevo=round()%(movimientos);
    int limite=(filas+columnas);

    int p_money=money_points();
    int p_builder=kill_builder_points();
    int p_warrior=kill_warrior_points();

    //vectores de objetos
    vector <pair <int, int> > v_pistola;
    vector <pair <int, int> > v_bazooka;
    vector <pair <int, int> > v_money;
    vector <pair <int, int> > v_food;
    vector <pair <int, int> > v_enemigo;

    //matriz del mapa
    vector < vector <casilla> > mat(filas, vector <casilla> (columnas));
    mapa(noche, mat,v_enemigo, v_bazooka,v_food,v_pistola,v_money);

    //indicie para saber que pos del vector de acciones globales le toca 

    int num_obj=v_pistola.size()+v_bazooka.size()+v_money.size()+v_food.size()+v_enemigo.size();
    int pos_vag=-1;
    vector <pair <bool, map < long double, accion, greater<long double> > > > acciones_globales;
    vector <int> vb=builders(me());
	  
	  
    //builders

    //huyen para distraer, construyen para molestar y cogen monedas

    for (int b : vb){
      if (citizen(b).life!=0){
      pos_vag++;
      int cont_obj=0;
      unsigned int iter=0;

      vector <pair<int, int> > v_pos;
      v_pos.push_back(make_pair(citizen(b).pos.i, citizen(b).pos.j));
      int i=v_pos[iter].first;
      int j=v_pos[iter].second;

      map <long double, accion ,greater<long double>> m_acciones;//map de acciones definidas por puntos, destino, ejecutor, direccion
      acciones_globales.push_back(make_pair(true, m_acciones));
      vector < vector <casilla> > m_unico(filas, vector <casilla> (columnas));//mapa de distancias individual de cada personaje nuestro
      m_unico=mat;
      m_unico[i][j].cost=0;
      accion ac_individual;
      Pos lejos;
      Pos mi_pos;
      mi_pos.i=i;
      mi_pos.j=j;
      bool camino=false;
      int dist=0;

      //bool accion_hecha=construir_barricada(acciones_globales, m_unico, i, j, b);
      //bool accion_hecha=const_barricadas(m_unico, i, j, b);
      bool accion_hecha=false;
      if (not accion_hecha){
        while (cont_obj<num_obj and iter<v_pos.size()){
          accion ac_individual;
          i=v_pos[iter].first;
          j=v_pos[iter].second;
          
          if (pos_ok(i, j) and m_unico[i][j].soy!="edificio"){
            lejos.i=i;
            lejos.j=j;
            camino=true;
            dist=m_unico[i][j].cost;
            string soc=m_unico[i][j].soy;
            if (soc!="persona" and soc!="suelo" ){
              ac_individual.dir=buscar_camino(m_unico, i, j);
              int su_vida=0;
              long double valor=determinar_valor_b(soc, dist, citizen(b).life, warrior_ini_life(), b);
              ac_individual.obj=soc;
              ac_individual.id=b;
              ac_individual.p.i=i;
              ac_individual.p.j=j;
              ac_individual.soy="builder";
              
              
              acciones_globales[pos_vag].second.insert(make_pair(valor,ac_individual));
            }
            bool construyo=false;
            if (is_day() and noche and soc=="enemigo" and dist<5 and citizen(cell(i, j).id).type==Warrior){
              construyo =const_barricadas(m_unico,  b);
              if (construyo) iter=v_pos.size();
            }
            if (dist<3 and noche and soc=="enemigo" and not construyo){
              bool esconderse=escondite(m_unico, b, mat);
              if(not esconderse){
                buscar_huida(m_unico, mi_pos ,lejos, b);
                iter=v_pos.size();
              }
            }
            else if (soc!="enemigo" and (not construyo) and iter!=v_pos.size()){ alrededor(v_pos, m_unico, i, j, b);}
            
          }

          iter++;
        }
        if (camino and iter>1 and dist<=limite){//no has huido
          ac_individual.dir=buscar_camino(m_unico, lejos.i, lejos.j);
          ac_individual.obj="suelo";
          ac_individual.id=b;
          ac_individual.p=lejos;
          ac_individual.soy="builder";
          acciones_globales[pos_vag].second.insert(make_pair(0,ac_individual));
        }
      }

      }
      

    }

    // warriors
      
      vector <int> vw=warriors(me());
	  
      for (int w :  vw){//recorro warrior....................................................................
        if (citizen(w).life!=0){
          pos_vag++;
          int cont_obj=0;
          unsigned int iter=0;
          vector <pair<int, int> > v_pos;
          v_pos.push_back(make_pair(citizen(w).pos.i, citizen(w).pos.j));
          int i=v_pos[iter].first;
          int j=v_pos[iter].second;

          map <long double, accion ,greater<long double>> m_acciones;//map de acciones definidas por puntos, objeto destino, ejecutor, direccion
          acciones_globales.push_back(make_pair(true, m_acciones));
          vector < vector <casilla> > m_unico(filas, vector <casilla> (columnas));//mapa de distancias individual de cada personaje nuestro
          m_unico=mat;
          m_unico[i][j].cost=0;
          
          int dist=0;
          accion ac_individual;
          Pos lejos;
          bool camino=false;
          Pos enemigo_pos;
          bool voy_a_por_el=true;
          while(cont_obj<num_obj and iter<v_pos.size()){//hasta que no haya encontrado el camino a todos los objetos alcanzables
            i=v_pos[iter].first;
            j=v_pos[iter].second;
            
            if (pos_ok(i,j) and m_unico[i][j].soy!="edificio"){
                  lejos.i=i;
                  lejos.j=j;
                  camino=true;
                  string soc=m_unico[i][j].soy;

                  dist=m_unico[i][j].cost;//ves a objetos cercanos.................................................................................
                  bool puedo_ir=true;
                  //me encuentro algo
                  if (soc!="persona" and soc!="suelo" ){
                    cont_obj++;
                    int id=cell(i, j).id;

                    if (soc=="enemigo") puedo_ir=huir_o_luchar(m_unico, id, w, i, j);  //control de fuerza
                    if (dist<3 and (2*life_lost_in_attack()>citizen(w).life) and voy_a_por_el==true) voy_a_por_el=puedo_ir;

                    if (soc=="pistola" and (citizen(w).weapon==Gun or citizen(w).weapon==Bazooka)) puedo_ir=false;//no vayas a por armas peores
                    
                    ac_individual.dir=buscar_camino(m_unico, i, j);
                    int su_vida=0;
                    if (soc=="enemigo"){//arreglar parametros para det. valor
                      su_vida=citizen(cell(i, j).id).life;
                      if(citizen(cell(i, j).id).type==Builder){
                        soc="builder";
                      }
                    }
                    ac_individual.obj=soc;
                    ac_individual.id=w;
                    ac_individual.p.i=i;
                    ac_individual.p.j=j;
                    ac_individual.soy="warrior";
                    long double valor=determinar_valor_w(soc, m_unico[i][j].cost, p_warrior, p_builder, p_money, citizen(w).life, su_vida, warrior_ini_life(),m_unico, w,i, j);
                    //if (puedo_ir) m_acciones.insert(make_pair(valor,ac_individual));
                    if (puedo_ir) acciones_globales[pos_vag].second.insert(make_pair(valor,ac_individual));

                    
                  }
                  //guardo como posible huida
                  if (voy_a_por_el==false and dist>0 and dist<3 and noche and soc=="enemigo"){//me voy de donde estoy, da igual lo que este haciendo
                    enemigo_pos=lejos;
                    dist=limite;
                  }
                  //miro si puedo ir al rededor
                  if (puedo_ir and soc!="enemigo" and soc!="builder" and (soc!="persona" or dist==0)) alrededor(v_pos, m_unico, i, j, w);
            }
            iter++;
          }
          if (camino and dist>0){
            long double value=0;
            

            ac_individual.obj="suelo";
            ac_individual.id=w;
            ac_individual.p=lejos;
            ac_individual.soy="warrior";

            if (voy_a_por_el==false){
              Pos mi_pos=citizen(w).pos;
              buscar_huida(m_unico, mi_pos, enemigo_pos, w);
            }
            else{ 
            ac_individual.dir=buscar_camino(m_unico, lejos.i, lejos.j);
            acciones_globales[pos_vag].second.insert(make_pair(value,ac_individual));
            }
          }
        }
          
      }





    gestor_de_acciones_globales(acciones_globales);
    
  }
};




/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
