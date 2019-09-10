using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PicaVoxel;

public class EnemyBulletPool : MonoBehaviour {


    #region Singelton
    public static EnemyBulletPool Instance;

    private void Awake()
    {
        Instance = this;
    }
    #endregion

    public GameObject objectToPool;
    public int amountToPool;
    public List<GameObject> pooledObjects;

    // Use this for initialization
    void Start () { 

        pooledObjects = new List<GameObject>();

            for(int i = 0; i < amountToPool; i++)
            {
                GameObject obj = Instantiate(objectToPool);
                obj.SetActive(false);
                obj.transform.parent = transform;
                pooledObjects.Add(obj);
                
            }

	}
	


    public GameObject GetPooledObject()
    {
        for(int i = 0; i< pooledObjects.Count; i++)
        {
            //Si está deshabilitado, está en el Pool
            if (!pooledObjects[i].activeInHierarchy)
            {
                return pooledObjects[i];
            }

        }

        GameObject obj = Instantiate(objectToPool);
        obj.SetActive(false);
        pooledObjects.Add(obj);
        obj.transform.parent = transform;
        return obj;
    }



    public GameObject SpawnFromPool(Vector3 position, Quaternion rotation)
    {
        GameObject objectToSpawn = GetPooledObject();
        if (objectToSpawn != null)
        { 
            objectToSpawn.transform.position = position;
            objectToSpawn.transform.rotation = rotation;
            objectToSpawn.SetActive(true);
        }
        

        return objectToSpawn;
    }

    public void RemoveAllBullets()
    {
        for (int i = 0; i < pooledObjects.Count; i++)
        {
            var bullet = pooledObjects[i];

            if (bullet != null)
            {
                if (bullet.activeInHierarchy)
                {
                    bullet.SetActive(false);
                }
            }
        }
    }

    public void ExplodeAllBullets()
    {
        for (int i = 0; i < pooledObjects.Count; i++)
        {
            var bullet = pooledObjects[i];

            if (bullet != null)
            {
                if (bullet.activeInHierarchy)
                {
                    //var simulated = Instantiate(bullet, bullet.transform.position, bullet.transform.rotation);

                    bullet.SetActive(false);

                    /*var simExplosion = simulated.GetComponent<Exploder>();
                    simExplosion.ExplodeTarget = Exploder.ExplodeTargets.SelfOnly;
                    simExplosion.Explode();
                    Destroy(simulated);*/
                }
            }
        }
    }

    public void ExplodeOnRadious(Vector3 position, float radious)
    {
        for (int i = 0; i < pooledObjects.Count; i++)
        {
            var bullet = pooledObjects[i];

            if (bullet != null)
            {
                if (bullet.activeInHierarchy)
                {
                    if(Vector3.Distance(position, bullet.transform.position) <= radious)
                    {
                        //var simulated = Instantiate(bullet, bullet.transform.position, bullet.transform.rotation);

                        bullet.SetActive(false);

                        /*var simExplosion = simulated.GetComponent<Exploder>();
                        simExplosion.ExplodeTarget = Exploder.ExplodeTargets.SelfOnly;
                        simExplosion.Explode();
                        Destroy(simulated);*/
                    }
                }
            }
        }
    }

    // Update is called once per frame
    void Update () {

        for (int i = 0; i < pooledObjects.Count; i++)
        {
            var bullet = pooledObjects[i];

            if (bullet != null && bullet.activeInHierarchy)
            {
                if(bullet.transform.position.y <= -1)
                {
                    bullet.SetActive(false);
                    AchivementManager.instance.AchivementBulletsAvoided();
                }
                else
                {
                    bullet.transform.Translate(transform.forward * 10 * Time.deltaTime);
                }
            }
                
        }

    }
}
