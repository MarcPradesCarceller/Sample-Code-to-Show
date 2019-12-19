using UnityEngine;
using System.Collections;

public class CubeMovement : MonoBehaviour
{
    #region Cube Movement

    [SerializeField]
    private float rotationPeriod = 0.3f;
    [SerializeField]
    private float sideLength = 1f;

    private bool isRotate = false;
    private float directionX = 0;
    private float directionZ = 0;

    private Vector3 startPos;
    private float rotationTime = 0;
    private float radius;
    private Quaternion fromRotation;
    private Quaternion toRotation;

    #endregion

    #region PathFinding
    [SerializeField]
    private Transform target;

    private const float minPathUpdateTime = 0.2f;
    private const float pathUpdateMoveThreshold = 0.5f;
    private Vector3[] path;
    private int targetIndex;
    private Vector3 vectorToFollow;

    private float x = 0;
    private float y = 0;
    private bool toTarget = false;

    private GameObject[] npcs;
    private Rigidbody rig;
    #endregion

    // Use this for initialization
    void Start()
    {
        radius = sideLength * Mathf.Sqrt(2f) / 2f;
        npcs = GameObject.FindGameObjectsWithTag("NPC");
        rig = this.GetComponent<Rigidbody>();
        StartCoroutine(UpdatePath());
    }

    // Update is called once per frame
    void Update()
    {

        if ((x != 0 || y != 0) && !isRotate)
        {
            directionX = y;
            directionZ = x;
            startPos = transform.position;
            fromRotation = transform.rotation;
            transform.Rotate(directionZ * 90, 0, directionX * 90, Space.World);
            toRotation = transform.rotation;
            transform.rotation = fromRotation;
            rotationTime = 0;
            isRotate = true;
        }
    }

    void FixedUpdate()
    {
        RotateCubeMovement();
    }

    //Code to make the cube rotate
    private void RotateCubeMovement()
    {
        if (isRotate)
        {

            rotationTime += Time.fixedDeltaTime;
            float ratio = Mathf.Lerp(0, 1, rotationTime / rotationPeriod);

            float thetaRad = Mathf.Lerp(0, Mathf.PI / 2f, ratio);
            float distanceX = -directionX * radius * (Mathf.Cos(45f * Mathf.Deg2Rad) - Mathf.Cos(45f * Mathf.Deg2Rad + thetaRad));
            float distanceY = radius * (Mathf.Sin(45f * Mathf.Deg2Rad + thetaRad) - Mathf.Sin(45f * Mathf.Deg2Rad));
            float distanceZ = directionZ * radius * (Mathf.Cos(45f * Mathf.Deg2Rad) - Mathf.Cos(45f * Mathf.Deg2Rad + thetaRad));
            transform.position = new Vector3(startPos.x + distanceX, startPos.y + distanceY, startPos.z + distanceZ);

            transform.rotation = Quaternion.Lerp(fromRotation, toRotation, ratio);

            if (ratio == 1)
            {
                isRotate = false;
                directionX = 0;
                directionZ = 0;
                rotationTime = 0;
            }
        }

    }

    public void OnPathFound(Vector3[] newPath, bool pathSuccessful)
    {
        if (pathSuccessful)
        {
            path = newPath;
            StopCoroutine("FollowPath");
            StartCoroutine("FollowPath");
        }
    }


    IEnumerator UpdatePath()
    {

        if (Time.timeSinceLevelLoad < .3f)
        {
            yield return new WaitForSeconds(.3f);
        }
        PathRequestManager.RequestPath(new PathRequest(transform.position, target.position, OnPathFound));

        float sqrMoveThreshold = pathUpdateMoveThreshold * pathUpdateMoveThreshold;
        Vector3 targetPosOld = target.position;

        while (true)
        {
            yield return new WaitForSeconds(minPathUpdateTime);

            //Aceptance radious for a stationary target
            if (objetivoFijo)
            {
                if ((target.position - targetPosOld).sqrMagnitude > sqrMoveThreshold)
                {
                    PathRequestManager.RequestPath(new PathRequest(transform.position, target.position, OnPathFound));
                    targetPosOld = target.position;
                    toTarget = false;
                }
            }
            //If the target is not stationri, there is no need of an aceptance radious
            else
            {
                PathRequestManager.RequestPath(new PathRequest(transform.position, target.position, OnPathFound));
                targetPosOld = target.position;
                toTarget = false;
            }
        }
    }

    IEnumerator FollowPath()
    {

        if (path.Length > 0)
        {
            Vector3 currentWaypoint = path[0];

            while (true)
            {
                //Check if has arrived to the target, whatever his coordinates are
                if (toTarget)
                {

                    Vector3 dist = this.transform.position - currentWaypoint;

                    if (dist.magnitude < 1)
                    {
                        targetIndex++;
                        if (targetIndex >= path.Length)
                        {
                            targetIndex = 0;
                            path = null;
                            toTarget = false;
                            x = 0;
                            y = 0;
                            yield break;
                        }
                        currentWaypoint = path[targetIndex];
                        toTarget = false;
                    }

                }

                //Calculate the axis direction
                if (!toTarget)
                {

                    vectorToFollow = currentWaypoint - gameObject.transform.position;
                    vectorToFollow = vectorToFollow.normalized;
                    x = vectorToFollow.z;
                    y = -vectorToFollow.x;
                    toTarget = true;
                }


                yield return null;
            }
        }

    }

    //DrawPath in Gizmos
    public void OnDrawGizmos()
    {
        if (path != null)
        {
            for (int i = targetIndex; i < path.Length; i++)
            {
                Gizmos.color = Color.black;
                Gizmos.DrawCube(path[i], Vector3.one);
                if (i == targetIndex)
                {
                    Gizmos.DrawLine(transform.position, path[i]);
                }
                else
                {
                    Gizmos.DrawLine(path[i - 1], path[i]);
                }
            }
        }
    }


}