import pickle
import sqli_dataSet
import dpkt
import sys
def sqli(payload):
    vlist=[]
    try:
        request = dpkt.http.Request(payload)
    except (dpkt.dpkt.NeedData, dpkt.dpkt.UnpackError):
        return False
    
    if (request.method=='GET') and ('?' in request.uri):
        for V in request.uri.split('?')[1:]:
            for param in V.split('&'):
                if '=' in param:
                    vlist.append(param.split('=')[1])
    elif (request.method=='POST'):
        for param in str(request.body.decode('utf-8')).split('&'):
            if '=' in param:
                vlist.append(param.split('=')[1])
    else:
        return False
    transformer=sqli_dataSet.TransformData()
    data=transformer.transform(vlist)
    sk_model=pickle.load(open('/home/jwh/master/Python/sqli.ml', 'rb'))
    answer=sk_model.predict(data)
    for x in answer:
        if x==1:
            return True
    else:
        return False
        

if __name__=='__main__':
    a=b'GET /board.php?bbs_id=notice&ax=%20or%20x=x HTTP/1.1\r\nHost: shoplogo.com\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nReferer: http://shoplogo.com/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\nCookie: PHPSESSID=58menjipsr9l8lv1aq75u9g997; scrollbox_tab=1; scrollbox_page1=1; popup_1=ok\r\n\r\n'
    print(DetectSQLi(a))
    '''
    x=0
    for ts, pkt in dpkt.pcap.Reader(open('normal.pcap', 'rb')):
        if(x==2):
            break;
        eth = dpkt.ethernet.Ethernet(pkt)
        if not isinstance(eth.data, dpkt.ip.IP):
            continue
        ip = eth.data
        if isinstance(ip.data, dpkt.tcp.TCP):
            tcp = ip.data
            try:
                request = dpkt.http.Request(tcp.data)
            except (dpkt.dpkt.NeedData, dpkt.dpkt.UnpackError):
                continue
            if (request.method=='GET') and ('?' in request.uri):
                x+=1
                print(tcp.data)'''

        
