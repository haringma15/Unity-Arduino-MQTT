using UnityEngine;

public class DB : MonoBehaviour
{
    [Header ("Data Categories")]
    [SerializeField] private ConfigData configData;
    [SerializeField] private HelperData helperData;
    [SerializeField] private MqttClientData mqttClientData;

    public static ConfigData Config;
    public static HelperData Helpers;
    public static MqttClientData MqttClient;

    void Awake()
    {
        if (alreadyInitialized) Destroy(gameObject);
        
        Config = configData;
        MqttClient = mqttClientData;
        Helpers = helperData;

        DontDestroyOnLoad(gameObject);
    }


    private bool alreadyInitialized => Config != null;
}
