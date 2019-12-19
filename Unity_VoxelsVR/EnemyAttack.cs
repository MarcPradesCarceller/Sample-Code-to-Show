using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAttack : MonoBehaviour {


    [SerializeField]
    private bool isKamikaze;
    [SerializeField]
    [Range(1f, 40f)]
    private float maxKamikazeTime;
    [SerializeField]
    [Range(1f, 40f)]
    private float minKamikazeTime;

    [SerializeField]
    private float kamikazeSpeed;

    [Space(20)]

    [SerializeField]
    private bool fireBullets;
    [SerializeField]
    private GameObject bullet;
    [SerializeField]
    [Range(1f, 40f)]
    private float maxShootTime;
    [SerializeField]
    [Range(1f, 40f)]
    private float minShootTime;

    [SerializeField]
    private float waitingBeforeShoot;
    [SerializeField]
    private float waitingAfterShoot;

    private int valuePoints;
    private bool beenHit;
    private int enemyType;

    [SerializeField]
    private Transform playerPosition;

    private Color attackColor;

    private bool hasShoot;

    BoxCollider col;
    public Renderer[] var;

    private CAudioManager audioManager;

    MaterialPropertyBlock props;
    EnemyBulletPool bulletPool;

    #region Getters&Setters
    public bool IsKamikaze
    {
        get
        {
            return isKamikaze;
        }

        set
        {
            isKamikaze = value;
        }
    }

    public float MaxKamikazeTime
    {
        get
        {
            return maxKamikazeTime;
        }

        set
        {
            maxKamikazeTime = value;
        }
    }

    public float MinKamikazeTime
    {
        get
        {
            return minKamikazeTime;
        }

        set
        {
            minKamikazeTime = value;
        }
    }

    public float KamikazeSpeed
    {
        get
        {
            return kamikazeSpeed;
        }

        set
        {
            kamikazeSpeed = value;
        }
    }

    public bool FireBullets
    {
        get
        {
            return fireBullets;
        }

        set
        {
            fireBullets = value;
        }
    }

    public float MaxShootTime
    {
        get
        {
            return maxShootTime;
        }

        set
        {
            maxShootTime = value;
        }
    }

    public float MinShootTime
    {
        get
        {
            return minShootTime;
        }

        set
        {
            minShootTime = value;
        }
    }

    public float WaitingBeforeShoot
    {
        get
        {
            return waitingBeforeShoot;
        }

        set
        {
            waitingBeforeShoot = value;
        }
    }

    public float WaitingAfterShoot
    {
        get
        {
            return waitingAfterShoot;
        }

        set
        {
            waitingAfterShoot = value;
        }
    }

    public int ValuePoints
    {
        get
        {
            return valuePoints;
        }

        set
        {
            valuePoints = value;
        }
    }

    public bool BeenHit
    {
        get
        {
            return beenHit;
        }

        set
        {
            beenHit = value;
        }
    }

    public Color AttackColor
    {
        get
        {
            return attackColor;
        }

        set
        {
            attackColor = value;
        }
    }

    public Transform PlayerPosition
    {
        get
        {
            return playerPosition;
        }

        set
        {
            playerPosition = value;
        }
    }

    public int EnemyType
    {
        get
        {
            return enemyType;
        }

        set
        {
            enemyType = value;
        }
    }

    #endregion

    // Use this for initialization
    void Start () {

        hasShoot = false;
        playerPosition = GameObject.FindGameObjectWithTag("MainCamera").transform;
        bulletPool = EnemyBulletPool.Instance;
        audioManager = FindObjectOfType<CAudioManager>();
        beenHit = false;

        if (IsKamikaze)
            StartCoroutine(KamikazeAttack());
        if(FireBullets)
            StartCoroutine(FirstShoot());

        props = new MaterialPropertyBlock();
 
    }

    private IEnumerator KamikazeAttack()
    {
        while (true)
        {
            yield return new WaitForSeconds(Random.Range(MinKamikazeTime, MaxKamikazeTime));
            //Crea otro vox colider y asigna su referencia, haciendo que el anterior nunca se borre si se ejecuta dos veces en el mismo viaje
            GetComponent<EnemyAxisMovement>().KamikazeAttackStart(KamikazeSpeed, playerPosition);
            yield return new WaitForSeconds(8f);

        }
    }

    private IEnumerator ShootAttack()
    {
        while (true)
        {
            yield return new WaitForSeconds(Random.Range(MinShootTime, MaxShootTime));
            Shoot();
        }
    }

    private IEnumerator FirstShoot()
    {
        float firstMax;

        if (MinShootTime > 10)
            firstMax = MinShootTime;
        else
            firstMax = 10;

        while (true)
        {
            yield return new WaitForSeconds(Random.Range(3f, firstMax));
            Shoot();
            StartCoroutine(ShootAttack());
            break;
        }
    }

    private void Shoot()
    {

        //Crea un collider en forma de tubo hacia delante para hacer que el resto de enemigos eviten colisionar con la trayectoria de la bala.
        //No llega hasta el jugador para evitar colisionar contra este
        float dist = Vector3.Distance(transform.position, transform.parent.position);
        col = gameObject.AddComponent<BoxCollider>();
        col.isTrigger = true;
        col.center = new Vector3(0, transform.lossyScale.y / 2, 0);
        col.size = new Vector3(2, 2, dist * 1.2f);


        //Change color
        ChangeColor(attackColor, 1);

        //anular movimiento
        gameObject.GetComponent<EnemyAxisMovement>().StopMovement();

        StartCoroutine(ResumeMovement());  

    }

    private IEnumerator ResumeMovement()
    {

            audioManager.PlayAtPont("EnemyCharge", transform.position);

            yield return new WaitForSeconds(WaitingBeforeShoot);

            Vector3 startPosition = new Vector3(transform.position.x, transform.position.y + transform.lossyScale.y / 2, transform.position.z);
            var b = bulletPool.SpawnFromPool(startPosition, transform.rotation);
            b.transform.LookAt(playerPosition);

            yield return new WaitForSeconds(WaitingAfterShoot);

            gameObject.GetComponent<EnemyAxisMovement>().ResumeMovement();

            //Change color
            ChangeColor(0);

            Destroy(col);

            yield break;

    }

    public void Enrage()
    {
        if (fireBullets)
        {
            minShootTime = minShootTime / 2;
            maxShootTime = maxShootTime / 2;

            //acelerar el siguiente ataque?

            waitingAfterShoot = waitingAfterShoot * 0.7f;
            waitingBeforeShoot = waitingBeforeShoot * 0.7f;
        }
    }

    private void ChangeColor(Color color, float value)
    {
        //props.SetColor("_Tint", color);
        props.SetColor("_FadeColor", color);
        props.SetFloat("_FadeAmount", value);
        foreach (Renderer r in var) r.SetPropertyBlock(props);
    }

    private void ChangeColor(float value)
    {
        props.SetFloat("_FadeAmount", value);
        foreach (Renderer r in var) r.SetPropertyBlock(props);
    }

}
