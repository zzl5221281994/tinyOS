;����������8�ֽ�

;  BYTE7         BYTE6 BYTE5                               BYTE4 BYTE3 BYTE2    BYTE1 BYTE0
;  �λ�ַ2     /   ������    \                              |    �λ�ַ1    |      �ν���
;  31~24      /               \                                23  ~    0           15~0
;            /                 \
;           /                   \
;         7   6   5  4       3 2 1 0    7  6 5  4  3 2 1 0
;         G D/B   0  AVL    |�ν���2|   P  DPL  S   TYPE
;                             19~16
;Pλ������λ���Ƿ����ڴ棬1��ʾ����     Gλ���ν������ȣ�0��ʾ����Ϊ�ֽڣ�1��ʾ����Ϊ4KB
;DPL����Ȩ����0~3 ��ԽС��Ȩ��Խ��      Sλ��S=1��ʾ���ݶ�/����Σ�S=0��ʾϵͳ��/��������
;D/B��   
;
;
;
;
;
;
SegDesc_Property_4KB  			EQU		8000H  ;4KB����
SegDesc_Property_32				EQU		4000H  ;32λ��

SegDesc_Property_DPL0			EQU		0000H  ;DPL����Ȩ��=0
SegDesc_Property_DPL1			EQU		0020H  ;DPL����Ȩ��=1
SegDesc_Property_DPL2			EQU		0040H  ;DPL����Ȩ��=2
SegDesc_Property_DPL3			EQU     0060H  ;DPL����Ȩ��=3
;�洢������������
SegDesc_Property_R 				EQU		0090H  ;���ڵ�ֻ�����ݶ�	
SegDesc_Property_RW				EQU		0092H  ;���ڵĿɶ�д���ݶ�
SegDesc_Property_RWA			EQU     0093H  ;���ڵ��ѷ��ʿɶ�д���ݶ�	
SegDesc_Property_EXEC			EQU		0098H  ;���ڵ�ִֻ�д����	
SegDesc_Property_EXEC_R 		EQU		009AH  ;���ڵĿ�ִ�пɶ������		
SegDesc_Property_EXEC_C			EQU		009CH  ;���ڵĿ�ִ��һ�´����	
SegDesc_Property_EXEC_RC		EQU		009EH  ;���ڵĿɶ���ִ��һ�´����	
;ϵͳ��������
SegDesc_Property_LDT			EQU		0082H  ;�ֲ�����������
SegDesc_Property_TaskGate		EQU		0085H  ;�����Ŷ�
SegDesc_Property_386TSS			EQU     0089H  ;386����״̬��
SegDesc_Property_386CGate		EQU		008CH  ;386�����Ŷ�
SegDesc_Property_386IGate		EQU		008EH  ;386�ж��Ŷ�
SegDesc_Property_TGate			EQU		008FH  ;386�����Ŷ�


;�ڴ�ֲ�ͼ���ݽṹ���