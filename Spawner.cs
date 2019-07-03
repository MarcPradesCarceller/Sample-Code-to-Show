using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PicaVoxel;


public class Spawner : MonoBehaviour {

    [SerializeField]
    private int actualWave;
    public List<Wave> waves;
    private int currentLevel;

    //Stores the last wave in wich a new Level was started
    //This is made to know the wave to restart.
    private int waveLevelRelation;

    private List<GameObject> enemyesArray;
    private List<GameObject> powerUpsSpawned;

    private bool enraged;
    private bool objectSpawned;

    //Theese numbers stores the thresholds for spawning power ups
    private int q1; //Quarter1
    private int q2; //Quarter2 (technically is 2 and 3)
    private int q3; //Quarter3 (in fact is Quarter 4)
    private List<PowerUpModifier> copiedList;

    public GameObject playerPosition;

    private GameManager manager;

    public int WaveLevelRelation
    {
        get
        {
            return waveLevelRelation;
        }

    }

    public int ActualWave
    {
        get
        {
            return actualWave;
        }

    }

    public int CurrentLevel
    {
        get
        {
            return currentLevel;
        }
    }

    // Use this for initialization
    void Start () {

        enemyesArray = new List<GameObject>();
        powerUpsSpawned = new List<GameObject>();
        manager = GetComponent<GameManager>();
        playerPosition = GameObject.FindObjectOfType<KamikazeDt>().gameObject;
        actualWave = 0;
        currentLevel = 0;
        enraged = false;
        
	}
	


    public void Spawn()
    {

        if(manager.State == GameManager.GameState.PLAYING)
        {

            if (waves[ActualWave].isNewLevel)
            {
                currentLevel++;
                waveLevelRelation = actualWave;

                PointsManager.instance.StopTimer();

                StartCoroutine(NewLevel());

                if (currentLevel % 5 == 0)
                {
                    AchivementManager.instance.ChechLevelAchivement(currentLevel);

                    if (currentLevel == 50)
                        FinalLevelReached();
                    else
                        manager.SaveLevelProgress(currentLevel, actualWave);
                }
                    
            }

            else
            {
                enraged = false;
                objectSpawned = false;
                StartCoroutine(SpawnCorutine());
            }
            
            
        }

    }

    public void ReestartLevel()
    {
        actualWave = waveLevelRelation;
        enraged = false;

        RemoveEnemies();
        currentLevel--;
        Spawn();
    }

    public void RemoveEnemies()
    {
        enraged = false;

        for (int i = enemyesArray.Count - 1; i >= 0; i--)
        {
            var delete = enemyesArray[i];
            enemyesArray.RemoveAt(i);
            Destroy(delete);
        }
    }

    private IEnumerator SpawnCorutine()
    {
        q1 = waves[ActualWave].enemies.Count / 4;
        q2 = waves[ActualWave].enemies.Count - q1;
        q3 = q1;
        //copiedList.Clear();
        copiedList = new List<PowerUpModifier>(waves[ActualWave].objectsToSpawn);


        yield return new WaitForSeconds(waves[ActualWave].timeBeforeStart);

        foreach (EnemyModifier e in waves[ActualWave].enemies)
        {
            GameObject enemy = Instantiate(e.enemy, new Vector3(20, 0, 0), transform.rotation);
            enemy.transform.parent = null;
            e.Apply(enemy);
            enemyesArray.Add(enemy);
        }


    }

    public void RemovePowerUp(GameObject g)
    {
        powerUpsSpawned.Remove(g);
    }

    public void RemoveAllPowerUps()
    {
        for (int i = powerUpsSpawned.Count - 1; i >= 0; i--)
        {
            var delete = powerUpsSpawned[i];
            powerUpsSpawned.Remove(powerUpsSpawned[i]);
            Destroy(delete);
        }
    }

    public void RemoveEnemy(GameObject g)
    {
        var pos = g.transform.GetChild(0).transform.position;

        if (enemyesArray.Remove(g))
        {

            SpawnPowerUp(pos);

            CheckIfFinish();
        }
            
    }


    private void SpawnPowerUp(Vector3 pos)
    {
        if (copiedList != null)
        {
            //por cada elemento
            for (int i = 0; i < copiedList.Count; i++)
            {
                var obj = copiedList[i];
                //si el objeto no ha sido spawneado
                if (obj != null)
                {
                    if (ChechIfMissingComponents(obj)) break;

                    //Spawn a un número concreto de enemigos derrotados
                    if (obj.priority == PowerUpModifier.Priority.SpawnAtEnemy)
                    {
                        if(waves[ActualWave].enemies.Count - enemyesArray.Count >= obj.spawnAtEnemyNo)
                        {
                            var pow = Instantiate(obj.powerUp, new Vector3(pos.x, pos.y + .8f, pos.z), Quaternion.identity);
                            obj.Apply(pow);
                            powerUpsSpawned.Add(pow);
                            copiedList[i] = null;
                            break;
                        }
                    }


                    //mirar su prioridad
                    if (obj.priority == PowerUpModifier.Priority.First)
                    {
                        //Aparecerá entre 0 y 1/4 de los enemigos
                        //El número va decreciendo con tal se mueren los enemigos, para que la probabilidad sea incremental
                        if (Random.Range(0, q1) == 0)
                        {
                            var pow = Instantiate(obj.powerUp, new Vector3(pos.x, pos.y + .8f,pos.z), Quaternion.identity);
                            obj.Apply(pow);
                            powerUpsSpawned.Add(pow);
                            copiedList[i] = null;
                            break;
                        }

                    }
                    if (obj.priority == PowerUpModifier.Priority.Middle)
                    {
                        //De esta forma no se empieza a spawnear esta clase de PowerUps hasta que no se llega a el Trheshold
                        if(enemyesArray.Count <= q2)
                        {
                            //Así empieza en el segundo cuarto, pero aparecerán antes del tercero seguro
                            if (Random.Range(0, q2-q3) == 0)
                            {
                                var pow = Instantiate(obj.powerUp, new Vector3(pos.x, pos.y + .8f, pos.z), Quaternion.identity);
                                obj.Apply(pow);
                                powerUpsSpawned.Add(pow);
                                copiedList[i] = null;
                                break;
                            }

                        }
                    }
                    if (obj.priority == PowerUpModifier.Priority.Last)
                    {
                        if (enemyesArray.Count <= q3)
                        {
                            if (Random.Range(0, q3) == 0)
                            {
                                var pow = Instantiate(obj.powerUp, new Vector3(pos.x, pos.y + .8f, pos.z), Quaternion.identity);
                                obj.Apply(pow);
                                powerUpsSpawned.Add(pow);
                                copiedList[i] = null;
                                break;
                            }
                        }
                    }
                    //Poniendo est el último, a causa de los break, causa que tengan mas prioridad de aparición los otros que este (ya que para eso se les ha asignado prioridad)
                    //La probabilidad también es incremental
                    //En el caso de que la diferencia de número entre enemigos y objetos sea muy pequeña (que no debería), es probable que los últimos power ups no lleguen a aparecer
                    //O esa es mi teoria (van hundiendose hasta que no les da tiempo a aparecer)
                    if (obj.priority == PowerUpModifier.Priority.Anytime)
                    {
                        if (Random.Range(0, enemyesArray.Count - 1) == 0)
                        {
                            var pow = Instantiate(obj.powerUp, new Vector3(pos.x, pos.y + .8f, pos.z), Quaternion.identity);
                            obj.Apply(pow);
                            powerUpsSpawned.Add(pow);
                            copiedList[i] = null;
                            break;
                        }
                    }

                    

                }//Fin obj != null

            }//Fin del bucle

            //Si q1-1 es mayor que cero, restamos uno
            if (q1 > 0) q1--;
            //Si es cero, significa que hemos pasado a los otros cuartos
            else
            {
                if (q2 - q3 > 0) q2--;
                else if (q3 > 0) { q3--; q2--; }
            }

        }
    }

    private bool ChechIfMissingComponents(PowerUpModifier obj)
    {
        if (obj.powerUp == null) { Debug.LogError("Object to spawn in " + obj.name + " is null"); return true; }
        if (obj.transparentMaterial == null) { Debug.LogError("Transparent material " + obj.name + " is null"); return true; }
        if (obj.timeOfLife == 0) { Debug.LogError("Time of life in " + obj.name + " is 0, object is destoryed on the same frame"); return true; }
        if(obj.type == PowerUp.Type.Heal && obj.healAmount == 0) { Debug.LogError("Heal amount in " + obj.name + " is 0, it won't heal"); return true; }
        if(obj.type == PowerUp.Type.Bomb && obj.radious == 0) { Debug.LogError("Explosion radious in " + obj.name + " is 0 (I have to put it automatically to 10)"); return true; }
        if(obj.priority == PowerUpModifier.Priority.SpawnAtEnemy && obj.spawnAtEnemyNo == 0) { Debug.LogError("Spawn at enemy No: in " + obj.name + " is 0, it will never spawn"); return true; }
        return false;
    }

    public void Explode(Vector3 position,float radious)
    {
        CAudioManager.instance.Play("Explosion");

        for (int i = enemyesArray.Count - 1; i >= 0; i--)
        {
            if(Vector3.Distance(position, enemyesArray[i].transform.GetChild(0).transform.position) <= radious)
            {

                var delete = enemyesArray[i];
                RemoveEnemy(delete);
                delete.GetComponentInChildren<Volume>().Destruct(1, true);
                Destroy(delete);

            }
        }
        EnemyBulletPool.Instance.ExplodeOnRadious(position, radious);
    }

    public void Apocalipsis()
    {
        CAudioManager.instance.Play("SuperExplosion");

        for (int i = enemyesArray.Count - 1; i >= 0; i--)
        {
                var delete = enemyesArray[i];
                RemoveEnemy(delete);
                delete.GetComponentInChildren<Volume>().Destruct(1, true);
                Destroy(delete);
        }
        EnemyBulletPool.Instance.ExplodeAllBullets();
    }

    private void CheckIfFinish()
    {
        if(enemyesArray.Count == 0)
        {

            if(ActualWave+1 > waves.Count - 1)
            {
                manager.GameFinished();
            }
            else
            {
                actualWave++;
                Spawn();
            }

        }

        else
        {
            if(enemyesArray.Count == waves[ActualWave].enrageWhenLeft && !enraged)
            {
                foreach(GameObject e in enemyesArray)
                {
                    if (e.GetComponentInChildren<EnemyAxisMovement>() == null) continue;
                    e.GetComponentInChildren<EnemyAxisMovement>().Enrage();
                }
                enraged = true;
            }
        }
    }

    private IEnumerator NewLevel()
    {
        manager.EnableFaceUI();
        manager.ChangeFaceUiText("");

        PointsManager.instance.ApplyPoints();

        yield return new WaitForSeconds(2f);

        manager.HideBonus();

        manager.ChangeFaceUiText("Level " + currentLevel.ToString());

        yield return new WaitForSeconds(4f);
        int time = 4;
        manager.ChangeFaceUiText(time.ToString());

        while (time > 0)
        {
            yield return new WaitForSeconds(1f);
            time--;
            manager.ChangeFaceUiText(time.ToString());
        }

        manager.DisableFaceUI();

        enraged = false;
        objectSpawned = false;

        //Una nueva ronda va a empezar, por lo que es necesario resetear los puntos y activar el timer
        PointsManager.instance.ResetTimer();
        PointsManager.instance.StartTimer();

        StartCoroutine(SpawnCorutine());

    }

    private void FinalLevelReached()
    {
        manager.FadeOutMusic();
        manager.ResetControllers();
    }

    public void SetWave(int wave, int level)
    {
        actualWave = wave;
        currentLevel = level;
    }

    



}
