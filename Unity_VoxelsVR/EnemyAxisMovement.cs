using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAxisMovement : MonoBehaviour {

    enum MovementType {  Soft, Abrupt, Mixed }

    [SerializeField]
    private MovementType movementType;

    [Header("Orbital Movement")]
    [SerializeField]
    [Range(.1f, 40f)]
    private float maxSpeed;
    [SerializeField]
    [Range(.1f, 40f)]
    private float minSpeed;

    [SerializeField]
    //La velocidad horizontal es menor que la vertical
    private float horizontalMultiplayer;

    //Interpola entre ambas velocidades para que el cambio sea suave
    private float currentSpeed;
    private float nextSpeed;

    [SerializeField]
    //Velocidad de la interpolación
    private float changeSpeed;

    [SerializeField]
    private float minChangeTime;
    [SerializeField]
    private float maxChangeTime;

    [Space(10)]

    private float kamikazeSpeed;

    private bool movingHorizontal;


    private bool isAvoiding;
    private bool canMove;

    private bool kamikazeAttack;
    [SerializeField]
    private AudioClip kamikazeClip;
    private AudioClip originalAudio;

    private Transform parentTransform;
    private Transform childTransform;

    private Coroutine orbit;

    private BoxCollider kamikazeColider;

    private Vector3 playerPosition;
    private Vector3 mirrorPosition;


    #region Getters&Setters
    public bool IsAvoiding
    {
        get
        {
            return isAvoiding;
        }

        set
        {
            OrbitSpeedSwitch();
            isAvoiding = value;
        }
    }

    public bool CanMove
    {
        get
        {
            return canMove;
        }

        set
        {
            canMove = value;
        }
    }

    public float MaxSpeed
    {
        get
        {
            return maxSpeed;
        }

        set
        {
            maxSpeed = value;
        }
    }

    public float MinSpeed
    {
        get
        {
            return minSpeed;
        }

        set
        {
            minSpeed = value;
        }
    }

    public float ChangeSpeed
    {
        get
        {
            return changeSpeed;
        }

        set
        {
            changeSpeed = value;
        }
    }

    public float MinChangeTime
    {
        get
        {
            return minChangeTime;
        }

        set
        {
            minChangeTime = value;
        }
    }

    public float MaxChangeTime
    {
        get
        {
            return maxChangeTime;
        }

        set
        {
            maxChangeTime = value;
        }
    }

    #endregion


    // Use this for initialization
    void Start()
    {

        childTransform = this.transform;
        parentTransform = this.transform.parent.transform;

        isAvoiding = false;
        canMove = true;
        kamikazeAttack = false;

        movingHorizontal = (Random.Range(0, 2) == 1);
        originalAudio = GetComponent<AudioSource>().clip;

        //Posición del objeto, aleatoria en altura y en profundidad
        parentTransform.position = Vector3.zero;
        if (GetComponent<EnemyAttack>().IsKamikaze)
        {
            childTransform.localPosition = new Vector3(0, Random.Range(1f, 8f), 18f);
        }
        else
            childTransform.localPosition = new Vector3(0, Random.Range(1f, 8f), Random.Range(9f, 18f));
        //Descomentar para probar colisiones
        //childTransform.localPosition = new Vector3(0, 5, 12);
        parentTransform.localEulerAngles = new Vector3(0, (float)Random.Range(0, 360) - 90, 0);
        
        childTransform.LookAt(parentTransform.position);

        orbit = StartCoroutine(OrbitSpeedModify());
        OrbitSpeedSwitch();
    }


    private void Update()
    {
        OrbitPlayer();

        if (kamikazeAttack)
        {

            //if (Vector3.Distance(childTransform.position, playerPosition) <= .2f)
            if (childTransform.position == playerPosition)
            {
                ReturnToMirrorPosition();
            }
            else
            {
                childTransform.position = Vector3.MoveTowards(childTransform.position, playerPosition, kamikazeSpeed * Time.deltaTime);
            }
        }
    }
        

    private void OrbitPlayer()
    {

        if(!kamikazeAttack)childTransform.LookAt(parentTransform.position);

        //Interpolación entre ambas velocidades
        if (currentSpeed != nextSpeed)
            currentSpeed = Mathf.Lerp(currentSpeed, nextSpeed, Time.deltaTime * ChangeSpeed);

        if (movingHorizontal)
        {
            if (canMove)
                parentTransform.localEulerAngles = SetOrbitRotation(currentSpeed * Time.deltaTime * horizontalMultiplayer, true);
            

            //Clamping
            if (parentTransform.localEulerAngles.y > 360f)
                parentTransform.localEulerAngles = SetOrbitRotation(parentTransform.localEulerAngles.y - 360);
            if (parentTransform.localEulerAngles.y < 0f)
                parentTransform.localEulerAngles = SetOrbitRotation(parentTransform.localEulerAngles.y + 360);
        }
        else
        {

            if(canMove)
                childTransform.localPosition = SetVerticalPosition(currentSpeed * Time.deltaTime, true);
            

            if (movementType == MovementType.Soft)
            {
                if (childTransform.localPosition.y >= 8f && currentSpeed >= 0)
                {
                    childTransform.localPosition = SetVerticalPosition(-currentSpeed * Time.deltaTime, true);
                    if (nextSpeed >= 0) nextSpeed = -nextSpeed;
                }

                if (childTransform.localPosition.y <= 0f && currentSpeed <= 0)
                {
                    childTransform.localPosition = SetVerticalPosition(-currentSpeed * Time.deltaTime, true);
                    if (nextSpeed <= 0) nextSpeed = -nextSpeed;
                }
            }

            if (movementType == MovementType.Abrupt)
            {
                if (childTransform.localPosition.y >= 8f && currentSpeed >= 0)
                {
                    currentSpeed = -currentSpeed;
                    if (nextSpeed >= 0) nextSpeed = -nextSpeed;
                }

                if (childTransform.localPosition.y <= 0f && currentSpeed <= 0)
                {
                    currentSpeed = -currentSpeed;
                    if (nextSpeed <= 0) nextSpeed = -nextSpeed;
                }
            }

            if (movementType == MovementType.Mixed)
            {
                if (childTransform.localPosition.y >= 8f && currentSpeed >= 0)
                {
                    OrbitSpeedSwitch();
                    if (nextSpeed >= 0) nextSpeed = -nextSpeed;
                    
                }

                if (childTransform.localPosition.y <= 0f && currentSpeed <= 0)
                {
                    OrbitSpeedSwitch();
                    if (nextSpeed <= 0) nextSpeed = -nextSpeed;

                }
            }
        }
    }


    private Vector3 SetOrbitRotation(float r, bool additive = false)
    {
        if (!additive)
            return new Vector3(parentTransform.localEulerAngles.x, r, parentTransform.localEulerAngles.z);
        else
            return new Vector3(parentTransform.localEulerAngles.x, parentTransform.localEulerAngles.y + r, parentTransform.localEulerAngles.z);
    }

    public IEnumerator OrbitSpeedModify()
    {
        while (true)
        {
            //Espera para recalcular la nueva velocidad
            yield return new WaitForSeconds(Random.Range(MinChangeTime, MaxChangeTime));
            if(canMove) OrbitSpeedSwitch();  
        }
    }

    public void OrbitSpeedSwitch()
    {
        do
        {
            //Calcula la nueva velocidad aleatoriamente
            nextSpeed = Random.Range(-MaxSpeed, MaxSpeed);
            movingHorizontal = !movingHorizontal;
            isAvoiding = false;

            //Si está entre estos valores, los cuales no quiero, vuelve a calcularlo
        } while (nextSpeed > -MinSpeed && nextSpeed < MinSpeed);
    }

    

    private Vector3 SetVerticalPosition(float r, bool additive = false)
    {
        if (!additive)
            return new Vector3(childTransform.localPosition.x, r, childTransform.localPosition.z);
        else
            return new Vector3(childTransform.localPosition.x, childTransform.localPosition.y + r, childTransform.localPosition.z);
    }

    public void AvoidCrash(Transform a, Transform b)
    {
        if (!isAvoiding && canMove)
        {

            Vector3 flee = a.position - b.position;
            flee = flee.normalized;

            if (movingHorizontal)
            {
                currentSpeed = MaxSpeed * flee.x;
                nextSpeed = currentSpeed * 2;
            }

            else
            {
                currentSpeed = MaxSpeed * flee.y;
                nextSpeed = currentSpeed * 2;
            }

            isAvoiding = true;
        }
    }

    public void Flee(Transform a, Transform b)
    {
        if (!isAvoiding && canMove)
        {

            Vector3 flee = a.position - b.position;
            flee = flee.normalized;

            if (movingHorizontal)
            {
                currentSpeed = MaxSpeed * flee.x * 2;
                nextSpeed = currentSpeed * 4;
            }

            else
            {
                currentSpeed = MaxSpeed * flee.y * 2;
                nextSpeed = currentSpeed * 4;
            }


            isAvoiding = true;
        }
    }

    public void KamikazeAttackStart(float k, Transform playerPos)
    {
        canMove = false;
        kamikazeSpeed = k;

        float dist = Vector3.Distance(transform.position, transform.parent.position);
        kamikazeColider = gameObject.AddComponent<BoxCollider>();
        kamikazeColider.isTrigger = true;
        kamikazeColider.center = new Vector3(0, transform.lossyScale.y / 2, dist / 2);
        kamikazeColider.size = new Vector3(2, 2, dist);

        var audio = GetComponent<AudioSource>();
        audio.Stop();
        audio.clip = kamikazeClip;
        audio.pitch = 1.3f;
        audio.Play();

        kamikazeAttack = true;
        playerPosition = new Vector3(playerPos.position.x, .8f, playerPos.position.z);
        mirrorPosition = new Vector3(-transform.position.x, transform.position.y, -transform.position.z);
        
    }


    private void ReturnToMirrorPosition()
    {
        kamikazeAttack = false;
        Destroy(kamikazeColider);

        var audio = GetComponent<AudioSource>();
        audio.Stop();
        audio.clip = originalAudio;
        audio.pitch = 1;

        audio.Play();

        StartCoroutine(MoveKamikazeOut());

    }
    private IEnumerator MoveKamikazeOut()
    {
     
        while (childTransform.position != mirrorPosition)
        {
            childTransform.position = Vector3.MoveTowards(childTransform.position, mirrorPosition, kamikazeSpeed * Time.deltaTime);
            yield return null;
        }
        canMove = true;
    }

    public void StopMovement()
    {
        canMove = false;
    }
    public void ResumeMovement()
    {
        canMove = true;
    }

    public void Enrage()
    {
        minSpeed = minSpeed * 2;
        maxSpeed = maxSpeed * 2;

        currentSpeed = currentSpeed * 2;
        nextSpeed = nextSpeed * 2;

        GetComponent<EnemyAttack>().Enrage();
    }


}
