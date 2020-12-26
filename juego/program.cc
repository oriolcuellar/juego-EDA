#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME oriol8






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
  void mapa(vector < vector <casilla> > &mat,vector<pair <int, int> > &v_enemigo,vector<pair <int, int> > &v_bazooka,vector<pair <int, int> > &v_food,vector<pair <int, int> > &v_pistola,vector<pair <int, int> > &v_money){
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
        if (cell(i,j).resistance!=-1 and cell(i,j).b_owner!=me()) mat[i][j].soy="barricada";

        if (is_day() and (mat[i][j].soy=="enemigo" or mat[i][j].soy=="barricada")) mat[i][j].soy="edificio";//si es dia no podemos mata ni tirar barricadas

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
  void alrededor(vector <pair<int, int> > &v_pos,vector < vector <casilla> > &m_unico , int i, int j){
      //marca las casillas de al rededor de una posicion.
      //dependiendo de lo que haya la visita o la marca invisitable
      if ( pos_ok(i+1, j) and m_unico[i+1][j].cost==-1 and m_unico[i+1][j].soy!="edificio"  and m_unico[i+1][j].soy!="persona"   and m_unico[i+1][j].soy!="huir"){
        v_pos.push_back(make_pair(i+1, j));
        m_unico[i+1][j].cost=m_unico[i][j].cost+1;
        m_unico[i+1][j].dir="arriba";
      }
        if (pos_ok(i-1, j) and m_unico[i-1][j].cost==-1 and m_unico[i-1][j].soy!="edificio" and m_unico[i-1][j].soy!="persona" and m_unico[i-1][j].soy!="huir"){
        v_pos.push_back(make_pair(i-1, j));
        m_unico[i-1][j].cost=m_unico[i][j].cost+1;
        m_unico[i-1][j].dir="abajo";
      }
      if (pos_ok(i, j+1) and m_unico[i][j+1].cost==-1 and m_unico[i][j+1].soy!="edificio" and m_unico[i][j+1].soy!="persona" and m_unico[i][j+1].soy!="huir"){
        v_pos.push_back(make_pair(i, j+1));
        m_unico[i][j+1].cost=m_unico[i][j].cost+1;
        m_unico[i][j+1].dir="izquierda";
      }
      if (pos_ok(i, j-1) and m_unico[i][j-1].cost==-1 and m_unico[i][j-1].soy!="edificio" and m_unico[i][j-1].soy!="persona" and m_unico[i][j-1].soy!="huir"){
        v_pos.push_back(make_pair(i, j-1));
        m_unico[i][j-1].cost=m_unico[i][j].cost+1;
        m_unico[i][j-1].dir="derecha";
    }
  }

  int determinar_valor_w(string soc, int dist, int warrior, int builder, int money, int mi_vida, int su_vida, int vida_total, int w){
    long int valor=-1;
    int quedan_rondas=(num_rounds_per_day()/2)-round()%(num_rounds_per_day()/2);
    if (soc=="enemigo" and is_night()){
      valor=warrior*1000;
    } 
    else if (soc=="builder" and is_night()) valor=builder*1000;
    else if (soc=="money") valor=money*1000;
    else if (soc=="bazooka"){
      valor=builder*2*1000;
      if (citizen(w).weapon!=Bazooka) valor =builder*4*1000;
      
    }
    else if (soc=="pistola"){
      valor=builder*1000;
    }
    else if (soc=="food" and mi_vida!=0){//falta aplicar rondas que quedan
      int div=0;
      div=(mi_vida*100)/vida_total;
      if (div<60){
        valor=(((builder+warrior)*100*1000)/2)/div;
      }
    }

    if (dist==0) dist=1;
    valor=(valor*1000)/(dist*dist);
    return valor;
  }


  int determinar_valor_b(string soc, int dist, int mi_vida, int vida_total){
    long int valor=-1;
    if (soc=="builder"){}
    else if(soc=="money") valor=money_points()*1000;
    else if (soc=="bazooka"){
      valor=money_points()*100;
    }
    else if (soc=="pistola"){
      valor=money_points()*10;
    }
    else if (soc=="food"){
      unsigned int div=0;
      div=(mi_vida*100)/vida_total;
      if (div<60){
        valor=(kill_builder_points()*100)/div;
      }
    }
    if (dist==0) dist=1;
    valor=(valor*1000)/(dist*dist);
    return valor;
  }



  void actuar(map < int, accion , greater<int> > m_acciones){
    map < int, accion , greater<int> > ::iterator it=m_acciones.begin();
    if (it!=m_acciones.end()){
    int id=it->second.id;
    Dir d;
    if (it -> second.dir =="arriba")d=Down;
    if (it -> second.dir =="abajo")d=Up;
    if (it -> second.dir =="derecha")d=Left;
    if (it -> second.dir =="izquierda")d=Right;
    move(id, d);
 
    }

  }



  bool huir_o_luchar(vector < vector <casilla> >  &m_unico, int id,int w,int i,int j){
    if (citizen(id).type!=Warrior) return true;
    bool puedo_ir=true;
    if (citizen(id).weapon==Bazooka and (citizen(w).weapon!=Bazooka or (citizen(w).life<=citizen(id).life)))puedo_ir=false;
    if (citizen(id).weapon==Gun and (citizen(w).weapon==Hammer or (citizen(w).weapon==Gun and citizen(w).life<=citizen(id).life))) puedo_ir=false;
    if (citizen(id).weapon==Hammer and citizen(w).weapon==Hammer and citizen(w).life<citizen(id).life) puedo_ir=false;
    if (not puedo_ir){
      m_unico[i][j].soy="edificio";
      m_unico[i][j].cost=-1;
    }
    return puedo_ir;
  }


//......................................................................................................................
//......................................................................................................................


  void gestor_de_acciones_globales(vector <pair <bool, map < int, accion, greater<int> > > > &acciones_globales){
    map < int, accion, greater<int> > :: iterator it;
    set <Pos> :: iterator jt;
    set <Pos> v_pos;
    for (unsigned int i=0;i<acciones_globales.size();i++){

      long int val_max=-1;
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
  void construir_barricada(vector <pair <bool, map < int, accion, greater<int> > > > &acciones_globales, vector < vector <casilla> >  &m_unico,int i,int j, int b){
    if (cell(i,j).b_owner!=me() and max_num_barricades() > barricades(me()).size()){//no estoy en barricada y quedan por construir 

    if (pos_ok(i+1, j) and cell(i+1, j).resistance==-1 and m_unico[i+1][j].soy=="suelo" and pos_ok(i+1, j+1) and cell(i+1, j+1).type==Building and pos_ok(i+1, j-1) and cell(i+1, j-1).type==Building){
      Dir d;
      d=Down;
      build (b,d);
    }
    if (pos_ok(i-1, j) and cell(i-1, j).resistance==-1  and m_unico[i-1][j].soy=="suelo" and pos_ok(i-1, j+1) and cell(i-1, j+1).type==Building and pos_ok(i-1, j-1) and cell(i-1, j-1).type==Building){
      Dir d;
      d=Up;
      build (b,d);
    }
    if (pos_ok(i, j+1) and cell(i, j+1).resistance==-1  and m_unico[i][j+1].soy=="suelo" and pos_ok(i+1, j+1) and cell(i+1, j+1).type==Building and pos_ok(i-1, j+1) and cell(i-1, j+1).type==Building){
      Dir d;
      d=Right;
      build (b,d);
    }
    if (pos_ok(i, j-1) and cell(i, j-1).resistance==-1  and m_unico[i][j-1].soy=="suelo" and pos_ok(i+1, j-1) and cell(i+1, j-1).type==Building and pos_ok(i-1, j-1) and cell(i-1, j-1).type==Building){
      Dir d;
      d=Left;
      build (b,d);
    }
    }
    
    
  }


  void buscar_huida(vector < vector <casilla> >  &m_unico, Pos mi_pos , Pos &enemigo, int id){
    int i=mi_pos.i;
    int j=mi_pos.j;
    int x=i-enemigo.i;
    if (x<0)x=-x;
    cerr<<endl<<i<<endl<<enemigo.i<<endl<<x<<endl;
    int y=j-enemigo.j;
    if (y<0)y=-y;
    int dist =x+y;
    bool movido=false;
      if (pos_ok(i+1, j) and m_unico[i+1][j].soy!="edificio"  and m_unico[i+1][j].soy!="persona"  and m_unico[i+1][j].soy!="enemigo" and m_unico[i+1][j].soy!="barricada"  and m_unico[i+1][j].soy!="builder"){
        x=i+1-enemigo.i;
        if (x<0)x=-x;
        y=j-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist and movido==false){ move (id, Down); movido=true;}
      }
      if (pos_ok(i-1, j) and m_unico[i-1][j].soy!="edificio" and m_unico[i-1][j].soy!="persona" and m_unico[i-1][j].soy!="enemigo" and m_unico[i-1][j].soy!="barricada"  and m_unico[i-1][j].soy!="builder"){
        x=i-1-enemigo.i;
        if (x<0)x=-x;
        y=j-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist and movido==false){ move (id, Up); movido=true;}
      }
      if (pos_ok(i, j+1) and m_unico[i][j+1].soy!="edificio" and m_unico[i][j+1].soy!="persona" and m_unico[i][j+1].soy!="enemigo" and m_unico[i][j+1].soy!="baricada"  and m_unico[i][j+1].soy!="builder"){
        x=i-enemigo.i;
        if (x<0)x=-x;
        y=j+1-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist and movido==false){ move (id, Right); movido=true;}
      }
      if (pos_ok(i, j-1) and m_unico[i][j-1].soy!="edificio" and m_unico[i][j-1].soy!="persona" and m_unico[i][j-1].soy!="enemigo" and m_unico[i][j-1].soy!="barricada"  and m_unico[i][j-1].soy!="builder"){
        x=i-enemigo.i;
        if (x<0)x=-x;
        y=j-1-enemigo.j;
        if (y<0)y=-y;
        int aux =x+y;
        if (aux>dist and movido==false){ move (id, Left); movido=true;}
    }
  }

  //......................................................................................................................
//......................................................................................................................


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
	
	// If nearly out of time, do nothing.
    double st = status(me());
    if (st >= 0.9) return;

    double num=100/num_rounds ();//porcentaje por ronda

    if ((st*100)/round()>num){

    }



    //parametros
    int filas=board_rows();
    int columnas=board_cols();
    int movimientos=num_rounds_per_day()/2;
    int llevo=round()%(movimientos);

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
    mapa(mat,v_enemigo, v_bazooka,v_food,v_pistola,v_money);

    //indicie para saber que pos del vector de acciones globales le toca 

    int pos_vag=-1;
    vector <pair <bool, map < int, accion, greater<int> > > > acciones_globales;
    //empieza warriors
      
      vector <int> vw=warriors(me());
      int num_obj=v_pistola.size()+v_bazooka.size()+v_money.size()+v_food.size()+v_enemigo.size();
      for (int w :  vw){//recorro warrior....................................................................
          pos_vag++;
          int cont_obj=0;
          unsigned int iter=0;
          vector <pair<int, int> > v_pos;
          v_pos.push_back(make_pair(citizen(w).pos.i, citizen(w).pos.j));
          int i=v_pos[iter].first;
          int j=v_pos[iter].second;

          map <int, accion ,greater<int>> m_acciones;//map de acciones definidas por puntos, objeto destino, ejecutor, direccion
          acciones_globales.push_back(make_pair(true, m_acciones));
          vector < vector <casilla> > m_unico(filas, vector <casilla> (columnas));//mapa de distancias individual de cada personaje nuestro
          m_unico=mat;
          m_unico[i][j].cost=0;
          int limite=(filas+columnas);
          int dist=0;
          accion ac_individual;
          Pos lejos;
          bool camino=false;
          Pos enemigo_pos;
          bool voy_a_por_el=true;
          while(cont_obj<num_obj and iter<v_pos.size() and dist<limite){//hasta que no haya encontrado el camino a todos los objetos alcanzables
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
                    if (dist<3 and (2*life_lost_in_attack()>citizen(w).life)) voy_a_por_el=puedo_ir;

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
                    long int valor=determinar_valor_w(soc, m_unico[i][j].cost, p_warrior, p_builder, p_money, citizen(w).life, su_vida, warrior_ini_life(), w);
                    //if (puedo_ir) m_acciones.insert(make_pair(valor,ac_individual));
                    if (puedo_ir) acciones_globales[pos_vag].second.insert(make_pair(valor,ac_individual));

                    
                  }
                  //guardo como posible huida
                    if (voy_a_por_el==false and dist>0 and dist<3 and is_night() and soc=="enemigo"){//me voy de donde estoy, da igual lo que este haciendo
                      enemigo_pos=lejos;
                      dist=limite;
                    }
                  //miro si puedo ir al rededor
                  if (puedo_ir and soc!="enemigo" and soc!="barricada" and soc!="builder" and (soc!="persona" or dist==0)) alrededor(v_pos, m_unico, i, j);
            }
            iter++;
          }
          if (camino and dist>0){
            long int value=0;
            

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



    //builders

    //huyen para distraer, construyen para molestar y cogen monedas

    vector <int> vb=builders(me());
    for (int b : vb){
      pos_vag++;
      int cont_obj=0;
      unsigned int iter=0;

      vector <pair<int, int> > v_pos;
      v_pos.push_back(make_pair(citizen(b).pos.i, citizen(b).pos.j));
      int i=v_pos[iter].first;
      int j=v_pos[iter].second;

      map <int, accion ,greater<int>> m_acciones;//map de acciones definidas por puntos, destino, ejecutor, direccion
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
      int limite=(filas+columnas);
      construir_barricada(acciones_globales, m_unico, i, j, b);

      while (cont_obj<num_obj and iter<v_pos.size() and dist<limite){
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
            long int valor=determinar_valor_b(soc, dist, citizen(b).life, warrior_ini_life());
            ac_individual.obj=soc;
            ac_individual.id=b;
            ac_individual.p.i=i;
            ac_individual.p.j=j;
            ac_individual.soy="builder";
            
            
            acciones_globales[pos_vag].second.insert(make_pair(valor,ac_individual));
          }
          if (dist<3 and is_night() and soc=="enemigo"){
            buscar_huida(m_unico, mi_pos ,lejos, b);
            dist=limite+1;
          }
          else{ alrededor(v_pos, m_unico, i, j);}
          
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
    gestor_de_acciones_globales(acciones_globales);
    
  }
};




/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
