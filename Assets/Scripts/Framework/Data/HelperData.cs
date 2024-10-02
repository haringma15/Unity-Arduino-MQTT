using UnityEngine;

public class HelperData : MonoBehaviour
{
    [Header ("Application-scoped helper variables")]
    [HideInInspector] public BaseScene currentScene;
    [HideInInspector] public string testTopic = "Arduino-Unity-MQTT-Test";
}
