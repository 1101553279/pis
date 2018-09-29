enum pca_id_event{
    PCA_ID_IN_NONE,
    PCA_ID_IN_DIAG_78,
    PCA_ID_IN_DIAG_56,
    PCA_ID_IN_UIC_78,
    PCA_ID_IN_UIC_56,
    PCA_ID_IN_IP,
    PCA_ID_IN_BUT,
    PCA_ID_IN_PPT,
    MAX_PCA_ID_IN,
};

enum net_in_id{
    NET_IN_ID_OCC = 0,      // listen
    NET_IN_ID_UIC_IDLE,     // pa
    NET_IN_ID_CAB_LINK,     // cab link info
    NET_IN_ID_PCOM,          // dacu pecu com
    NET_IN_ID_DCOM,          // dacu pecu com
    MAX_NET_IN_ID,
};
