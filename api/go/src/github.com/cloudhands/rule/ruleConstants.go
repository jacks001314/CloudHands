package rule

type RuleName struct {

	Name string `json:name`
	Cname string `json:cname`
	Id    int    `json:id`
}

const (
	ArrSplit = ","
	RuleEngineName = "RuleEngine.json"
	RuleFileExtName = ".json"
	RuleGroupName = "RuleGroup.json"
)

/*protos*/
const (
	/*
	 * rule proto names
	 * */
	ProtoHttp = "http"
	ProtoDns = "dns"
	ProtoTcp = "tcp"
    ProtoUdp= "udp"
    ProtoMail= "mail"
    ProtoFtp = "ftp"
    ProtoSSH = "ssh"
    ProtoTelnet = "telnet"
    ProtoSmon = "smon"
    ProtoPkt = "pkt"
)



const (
	/*rule op names*/
	OPcontains = "contains"
	OPstartsWith = "startsWith"
	OPendsWith = "endsWith"
	OPregex = "regex"
	OPeq = "eq"
	OPlt = "lt"
	OPgt = "gt"
	OPle = "le"
	OPge = "ge"
)

const (

	/*rule op name ids*/
	OPcontainsId = iota
	OPstartsWithId
	OPendsWithId
	OPregexId
	OPeqId
	OPltId
	OPgtId
	OPleId
	OPgeId
)

var OPIdMap = map[string]int{

	OPcontains:OPcontainsId,
	OPstartsWith:OPstartsWithId,
	OPendsWith:OPendsWithId,
	OPregex:OPregexId,
	OPeq:OPeqId,
	OPlt:OPltId,
	OPgt:OPgtId,
	OPle:OPleId,
	OPge:OPgeId,
}

var OPNameMap = []RuleName {

	{Name:OPcontains,Cname:"包含",Id:OPcontainsId},
	{Name:OPstartsWith,Cname:"以**开头",Id:OPstartsWithId},
	{Name:OPendsWith,Cname:"以**为结尾",Id:OPendsWithId},
	{Name:OPregex,Cname:"正则匹配",Id:OPregexId},
	{Name:OPeq,Cname:"等于",Id:OPeqId},
	{Name:OPlt,Cname:"小于",Id:OPltId},
	{Name:OPgt,Cname:"大于",Id:OPgtId},
	{Name:OPle,Cname:"小于等于",Id:OPleId},
	{Name:OPge,Cname:"大于等于",Id:OPgeId},
}

const (

	/*target common names*/
	SrcIP ="srcIP"
	SrcPort = "srcPort"
	DstIP ="dstIP"
	DstPort = "dstPort"

	/*target names for http session*/
	HttpMethod = "method"
	HttpUri = "uri"
	HttpExtName = "extName"
	HttpProto = "proto"
	HttpFUri = "furi"
	HttpHost = "host"
	HttpUseAgent = "ua"
	HttpStatus = "status"
	HttpReqHeader = "reqHead"
	HttpResHeader = "resHead"
	HttpReqBody = "reqBody"
	HttpResBody = "resBody"

	/*target name for dns*/
	DNsDomain = "domain"

	/*target name for tcp stream session*/
	TcpReqDataSize = "reqDataSize"
	TcpResDataSize = "resDataSize"
	TcpReqData = "reqData"
	TcpResData = "resData"

	/*target name for mail session*/
	MailUser = "user"
	MailPasswd = "passwd"
	MailFromMail = "fromMail"
	MailToMails = "toMails"
	MailCCMails = "ccMails"
	MailTitle = "title"
	MailContent = "content"
	MailAttachNames = "attachNames"

	/*target name for session monitor*/
	SmonMID = "id"
	SMonMProto = "mproto"
	SMonMReqData = "mreqData"
	SmonMResData = "mresData"

	/*target name for ftp*/
	FTPUser = "loginUser"
	FTPPasswd = "loginPasswd"
	FtpLoginCode = "loginCode"
	FtpCmds = "cmds"
	FtpCmdArgs = "cmdArgs"
	FtpCmdResCodes = "cmdCode"
	FtpCmdResPhrase = "cmdPhrase"

	/*
	 *ssh target names
	 * */
	SSHClientVersion = "cversion"
    SSHServerVersion = "sversion"
    SSHClientPkts = "cpkts"
	SSHServerPkts = "spkts"
	SSHClientBytes = "cbytes"
	SSHServerBytes = "sbytes"
	SSHClientEncPkts = "cEncPkts"
	SSHServerEncPkts = "sEncPkts"
	SSHClientEncBytes = "cEncBytes"
	SSHServerEncBytes = "sEncBytes"
	SSHClientEncMinBytes = "cEncMinBytes"
	SSHServerEncMinBytes = "sEncMinBytes"
	SSHClientEncMaxBytes = "cEncMaxBytes"
	SSHServerEncMaxBytes = "sEncMaxBytes"

	/*telnet target names*/

	TelnetUser = "Tuser"
	TelnetPasswd = "Tpasswd"
	TelnetContent = "Tcontent"

	/*packet target names*/
	PktL3Proto = "pktL3Proto"
	PktL4Proto = "pktL4Proto"
	PktDataSize = "pktDataSize"
	PktData = "pktData"
	PktL2HeaderSize="pktL2HeaderSize"
	PktL2Header = "pktL2Header"
	PktL3HeaderSize = "pktL3HeaderSize"
	PktL3Header = "pktL3Header"
	PktL4HeaderSize = "pktL4HeaderSize"
	PktL4Header = "pktL4Header"
	PktPayLoadSize = "pktPayLoadSize"
	PktPayload = "pktPayload"
	PktType = "pktType"
)

const (

	/*target common name ids*/
	SrcIPId = iota
	SrcPortId
	DstIPId
	DstPortId

	/*target name ids for http session*/
	HttpMethodId
	HttpUriId
	HttpExtNameId
	HttpProtoId
	HttpFUriId
	HttpHostId
	HttpUseAgentId
	HttpStatusId
	HttpReqHeadId
	HttpResHeadId
	HttpReqBodyId
	HttpResBodyId

	/*target name ids for dns session*/
	DnsDomainId

	/*target name ids for tcp session*/
	TcpReqDataSizeId
	TcpResDataSizeId
	TcpReqDataId
	TcpResDataId

	/*target name ids for mail session*/
	MailUserId
	MailPasswdId
	MailFromMailId
	MailToMailsId
	MailCCMailsId
	MailTitleId
	MailContentId
	MailAttachNamesId

	/*target name ids for session monitor*/
	SmonMIDId
	SMonMProtoId
	SMonMReqDataId
	SmonMResDataId

	/*target name ids for ftp*/
	FTPUserId
	FTPPasswdId
	FtpLoginCodeId
	FtpCmdsId
	FtpCmdArgsId
	FtpCmdResCodesId
	FtpCmdResPhraseId

	/*target name ids for ssh*/
	SSHClientVersionId
	SSHServerVersionId
	SSHClientPktsId
	SSHServerPktsId
	SSHClientBytesId
	SSHServerBytesId
	SSHClientEncPktsId
	SSHServerEncPktsId
	SSHClientEncBytesId
	SSHServerEncBytesId
	SSHClientEncMinBytesId
	SSHServerEncMinBytesId
	SSHClientEncMaxBytesId
	SSHServerEncMaxBytesId

	/*target name ids for telnet*/
	TelnetUserId
	TelnetPasswdId
	TelnetContentId

	/*target name ids for packet*/
	PktL3ProtoId
	PktL4ProtoId
	PktDataSizeId
	PktDataId
	PktL2HeaderSizeId
	PktL2HeaderId
	PktL3HeaderSizeId
	PktL3HeaderId
	PktL4HeaderSizeId
	PktL4HeaderId
	PktPayLoadSizeId
	PktPayloadId
	PktTypeId
)

/*map for names to ids*/
var TargetsMap = map[string]int{
	SrcIP:SrcIPId,
	SrcPort:SrcPortId,
	DstIP:DstIPId,
	DstPort:DstPortId,
	HttpMethod:HttpMethodId,
	HttpUri:HttpUriId,
	HttpExtName:HttpExtNameId,
	HttpProto:HttpProtoId,
	HttpFUri:HttpFUriId,
	HttpHost:HttpHostId,
	HttpUseAgent:HttpUseAgentId,
	HttpStatus:HttpStatusId,
	HttpReqHeader:HttpReqHeadId,
	HttpResHeader:HttpResHeadId,
	HttpReqBody:HttpReqBodyId,
	HttpResBody:HttpResBodyId,
	DNsDomain:DnsDomainId,
	TcpReqDataSize:TcpReqDataSizeId,
	TcpResDataSize:TcpResDataSizeId,
	TcpReqData:TcpReqDataId,
	TcpResData:TcpResDataId,
	MailUser:MailUserId,
	MailPasswd:MailPasswdId,
	MailFromMail:MailFromMailId,
	MailToMails:MailToMailsId,
	MailCCMails:MailCCMailsId,
	MailTitle:MailTitleId,
	MailContent:MailContentId,
	MailAttachNames:MailAttachNamesId,
	SmonMID:SmonMIDId,
	SMonMProto:SMonMProtoId,
	SMonMReqData:SMonMReqDataId,
	SmonMResData:SmonMResDataId,
	FTPUser:FTPUserId,
	FTPPasswd:FTPPasswdId,
	FtpLoginCode:FtpLoginCodeId,
	FtpCmds:FtpCmdsId,
	FtpCmdArgs:FtpCmdArgsId,
	FtpCmdResCodes:FtpCmdResCodesId,
	FtpCmdResPhrase:FtpCmdResPhraseId,
	SSHClientVersion:SSHClientVersionId,
	SSHServerVersion:SSHServerVersionId,
	SSHClientPkts:SSHClientPktsId,
	SSHServerPkts:SSHServerPktsId,
	SSHClientBytes:SSHClientBytesId,
	SSHServerBytes:SSHServerBytesId,
	SSHClientEncPkts:SSHClientEncPktsId,
	SSHServerEncPkts:SSHServerEncPktsId,
	SSHClientEncBytes:SSHClientEncBytesId,
	SSHServerEncBytes:SSHServerEncBytesId,
	SSHClientEncMinBytes:SSHClientEncMinBytesId,
	SSHServerEncMinBytes:SSHServerEncMinBytesId,
	SSHClientEncMaxBytes:SSHClientEncMaxBytesId,
	SSHServerEncMaxBytes:SSHServerEncMaxBytesId,
	TelnetUser:TelnetUserId,
	TelnetPasswd:TelnetPasswdId,
	TelnetContent:TelnetContentId,
	PktL3Proto:PktL3ProtoId,
	PktL4Proto:PktL4ProtoId,
	PktDataSize:PktDataSizeId,
	PktData:PktDataId,
	PktL2HeaderSize:PktL2HeaderSizeId,
	PktL2Header:PktL2HeaderId,
	PktL3HeaderSize:PktL3HeaderSizeId,
	PktL3Header:PktL3HeaderId,
	PktL4HeaderSize:PktL4HeaderSizeId,
	PktL4Header:PktL4HeaderId,
	PktPayLoadSize:PktPayLoadSizeId,
	PktPayload:PktPayloadId,
	PktType:PktTypeId,
}

/*map for protos to rulenames*/
var ProtoMap = map[string][]RuleName {

	ProtoHttp:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:HttpMethod,Cname:"请求方法",Id:HttpMethodId},
		{Name:HttpUri,Cname:"请求uri",Id:HttpUriId},
		{Name:HttpExtName,Cname:"文件扩展名",Id:HttpExtNameId},
		{Name:HttpProto,Cname:"http协议版本",Id:HttpProtoId},
		{Name:HttpFUri,Cname:"全uri",Id:HttpFUriId},
		{Name:HttpHost,Cname:"目标主机",Id:HttpHostId},
		{Name:HttpUseAgent,Cname:"User-Agent",Id:HttpUseAgentId},
		{Name:HttpStatus,Cname:"响应状态码",Id:HttpStatusId},
		{Name:HttpReqHeader,Cname:"请求头",Id:HttpReqHeadId},
		{Name:HttpResHeader,Cname:"响应头",Id:HttpResHeadId},
		{Name:HttpReqBody,Cname:"请求体",Id:HttpReqBodyId},
		{Name:HttpResBody,Cname:"响应体",Id:HttpResBodyId},
	},

	ProtoDns:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:DNsDomain,Cname:"请求域名",Id:DnsDomainId},
	},

	ProtoTcp:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:TcpReqDataSize,Cname:"请求数据大小",Id:TcpReqDataSizeId},
		{Name:TcpResDataSize,Cname:"响应数据大小",Id:TcpResDataSizeId},
		{Name:TcpReqData,Cname:"请求数据内容",Id:TcpReqDataId},
		{Name:TcpResData,Cname:"响应数据内容",Id:TcpResDataId},
	},

	ProtoUdp:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:TcpReqDataSize,Cname:"请求数据大小",Id:TcpReqDataSizeId},
		{Name:TcpResDataSize,Cname:"响应数据大小",Id:TcpResDataSizeId},
		{Name:TcpReqData,Cname:"请求数据内容",Id:TcpReqDataId},
		{Name:TcpResData,Cname:"响应数据内容",Id:TcpResDataId},
	},

	ProtoMail:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:MailUser,Cname:"用户名",Id:MailUserId},
		{Name:MailPasswd,Cname:"登录密码",Id:MailPasswdId},
		{Name:MailFromMail,Cname:"发件人邮箱",Id:MailFromMailId},
		{Name:MailToMails,Cname:"收件人邮箱",Id:MailToMailsId},
		{Name:MailCCMails,Cname:"抄送人邮箱",Id:MailCCMailsId},
		{Name:MailTitle,Cname:"邮件标题",Id:MailTitleId},
		{Name:MailContent,Cname:"邮件正文",Id:MailContentId},
		{Name:MailAttachNames,Cname:"附件名称",Id:MailAttachNamesId},
	},

	ProtoSmon:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:SmonMID,Cname:"监控任务id",Id:SmonMIDId},
		{Name:SMonMProto,Cname:"监控协议类型",Id:SMonMProtoId},
		{Name:SMonMReqData,Cname:"监控会话请求体",Id:SMonMReqDataId},
		{Name:SmonMResData,Cname:"监控会话响应体",Id:SmonMResDataId},
	},

	ProtoFtp:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:FTPUser,Cname:"ftp登录用户",Id:FTPUserId},
		{Name:FTPPasswd,Cname:"ftp登录密码",Id:FTPPasswdId},
		{Name:FtpLoginCode,Cname:"ftp登录状态码",Id:FtpLoginCodeId},
		{Name:FtpCmds,Cname:"ftp命令",Id:FtpCmdsId},

		{Name:FtpCmdArgs,Cname:"ftp命令参数",Id:FtpCmdArgsId},
		{Name:FtpCmdResCodes,Cname:"ftp命令响应码",Id:FtpCmdResCodesId},
		{Name:FtpCmdResPhrase,Cname:"ftp命令响应短语",Id:FtpCmdResPhraseId},
	},

	ProtoSSH:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:SSHClientVersion,Cname:"ssh客户端软件版本",Id:SSHClientVersionId},
		{Name:SSHClientPkts,Cname:"ssh客户端发送的数据包数",Id:SSHClientPktsId},
		{Name:SSHClientBytes,Cname:"ssh客户端发送的数据包字节数",Id:SSHClientBytesId},
		{Name:SSHClientEncPkts,Cname:"ssh客户端发送的加密包数",Id:SSHClientEncPktsId},

		{Name:SSHClientEncBytes,Cname:"ssh客户端发送的加密包字节数",Id:SSHClientEncBytesId},
		{Name:SSHClientEncMinBytes,Cname:"ssh客户端发送的最小加密包字节数",Id:SSHClientEncMinBytesId},
		{Name:SSHClientEncMaxBytes,Cname:"ssh客户端发送的最大加密包字节数",Id:SSHClientEncMaxBytesId},

		{Name:SSHServerVersion,Cname:"ssh服务端软件版本",Id:SSHServerVersionId},
		{Name:SSHServerPkts,Cname:"ssh服务端发送的数据包数",Id:SSHServerPktsId},
		{Name:SSHServerBytes,Cname:"ssh服务端发送的数据包字节数",Id:SSHServerBytesId},
		{Name:SSHServerEncPkts,Cname:"ssh服务端发送的加密包数",Id:SSHServerEncPktsId},
		{Name:SSHServerEncBytes,Cname:"ssh服务端发送的加密包字节数",Id:SSHServerEncBytesId},
		{Name:SSHServerEncMinBytes,Cname:"ssh服务端发送的最小加密包字节数",Id:SSHServerEncMinBytesId},
		{Name:SSHServerEncMaxBytes,Cname:"ssh服务端发送的最大加密包字节数",Id:SSHServerEncMaxBytesId},

	},

	ProtoTelnet:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:TelnetUser,Cname:"telnet登录用户名",Id:TelnetUserId},
		{Name:TelnetPasswd,Cname:"telnet登录密码",Id:TelnetPasswdId},
		{Name:TelnetContent,Cname:"telnet会话内容",Id:TelnetContentId},

	},

	ProtoPkt:{
		{Name:SrcIP,Cname:"源IP",Id:SrcIPId},
		{Name:DstIP,Cname:"目的IP",Id:DstIPId},
		{Name:SrcPort,Cname:"源端口",Id:SrcPortId},
		{Name:DstPort,Cname:"目的端口",Id:DstPortId},
		{Name:PktL3Proto,Cname:"网络层协议",Id:PktL3ProtoId},
		{Name:PktL4Proto,Cname:"传输层协议",Id:PktL4ProtoId},
		{Name:PktDataSize,Cname:"包大小",Id:PktDataSizeId},
		{Name:PktData,Cname:"包内容",Id:PktDataId},

		{Name:PktL2HeaderSize,Cname:"链路层头部大小",Id:PktL2HeaderSizeId},
		{Name:PktL2Header,Cname:"链路层头部内容",Id:PktL2HeaderId},
		{Name:PktL3HeaderSize,Cname:"网络层头部大小",Id:PktL3HeaderSizeId},

		{Name:PktL3Header,Cname:"网络层头部内容",Id:PktL3HeaderId},
		{Name:PktL4HeaderSize,Cname:"传输层头部大小",Id:PktL4HeaderSizeId},
		{Name:PktL4Header,Cname:"传输层头部内容",Id:PktL4HeaderId},
		{Name:PktPayLoadSize,Cname:"包载荷大小",Id:PktPayLoadSizeId},
		{Name:PktPayload,Cname:"包载荷内容",Id:PktPayloadId},
		{Name:PktType,Cname:"包类型",Id:PktTypeId},
	},
}


func GetProtoList() []string {
	return []string {ProtoTcp,ProtoUdp,ProtoHttp,ProtoDns,ProtoMail,ProtoFtp,ProtoSSH,ProtoTelnet,ProtoSmon,ProtoPkt}
}

func GetTargets(proto string) []RuleName {
	return ProtoMap[proto]
}

func GetOPs() []RuleName {
	return OPNameMap
}

func GetOPId (op string) int {

	return OPIdMap[op]
}

func GetTargetId(target string) int {

	return TargetsMap[target]

}

