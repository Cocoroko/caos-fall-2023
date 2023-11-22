### CURL и о том, как распарсить URL

статья о том, что такое URI и более детальный разбор URI: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/Identifying_resources_on_the_Web

чтобы поупражняться, можно получить токен яндекс.погоды и покрутить запросы: 
https://yandex.ru/dev/weather/doc/dg/concepts/forecast-info.html#req-format

или вот так: 


API Google Search: https://developers.google.com/webmaster-tools/about?hl=ru


API Yandex Search: https://yandex.ru/dev/sitesearch/sitesearchapi/?ysclid=lp9s2gg851744155928


примерчики: 

`
curl -X GET -I harvardsucks.org
`
`
curl -X GET -I https://www.google.com/search\?q=cat
`
`
curl -X POST "https://httpbin.org/post" \
-H "accept: application/json" \
-H "Content-Type: text/plain" \
-H "Custom-Header: Testing" \
-d "I love hashnode" \
-D result.headers \
-o result.json
`
пс: чтобы полученный от сервера json отображался в читаемом виде, можно использовать `json_pp`
