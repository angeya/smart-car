package org.angeya.websocket;

import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

/**
 * @author: angeya
 * @date: 2024/10/11 22:09
 * @description:
 */
@Slf4j
@Component
public class WebSocketHandler extends TextWebSocketHandler {

    /**
     * 会话集合
     */
    private static final Set<WebSocketSession> SESSION_SET = new HashSet<>();

    // 处理收到的文本消息
    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        String msg = message.getPayload();
        System.out.println("Received: " + msg);

        // 向客户端发送响应消息
        session.sendMessage(new TextMessage("Server received: " + msg));
        this.broadcastMsg(session, msg);
    }

    // 处理WebSocket连接建立
    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        System.out.println("建立连接: " + session.getId());
        SESSION_SET.add(session);
    }

    // 处理WebSocket连接关闭
    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        System.out.println("断开连接: " + session.getId());
        SESSION_SET.remove(session);
    }

    /**
     * 广播消息，除了当前会话
     * @param selfSession 当前session
     * @param msg 消息
     */
    public void broadcastMsg(WebSocketSession selfSession, String msg) {
        TextMessage textMessage = new TextMessage(msg);
        SESSION_SET.forEach(session -> {
            if (session == selfSession) {
                return;
            }
            try {
                session.sendMessage(textMessage);
            } catch (Exception e) {
                log.error("发送消息失败, 消息是:{}, 客户端是:{}", textMessage, session, e);
            }
        });
    }

    /**
     * 广播消息
     * @param msg 消息
     */
    public void broadcastMsg(String msg) {
        TextMessage textMessage = new TextMessage(msg);
        SESSION_SET.forEach(session -> {
            try {
                session.sendMessage(textMessage);
            } catch (Exception e) {
                log.error("发送消息失败, 消息是:{}, 客户端是:{}", textMessage, session, e);
            }
        });
    }
}
